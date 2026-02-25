#include "../lib/WheelController.hpp"
#include <HardwareSerial.h>
#include <cstdint>
#include <driver/ledc.h>

namespace WheelController {

// =====================
// Motor pin definitions
// =====================
static constexpr gpio_num_t LEFT_MOTOR_PIN0 = (gpio_num_t)33;
static constexpr gpio_num_t LEFT_MOTOR_PIN1 = (gpio_num_t)32;
static constexpr gpio_num_t RIGHT_MOTOR_PIN0 = (gpio_num_t)26;
static constexpr gpio_num_t RIGHT_MOTOR_PIN1 = (gpio_num_t)25;

// =====================
// PWM settings
// =====================
static constexpr ledc_timer_t LEDC_TIMER = LEDC_TIMER_0;
static constexpr ledc_mode_t LEDC_MODE = LEDC_LOW_SPEED_MODE;
static constexpr ledc_timer_bit_t LEDC_DUTY_RES = LEDC_TIMER_8_BIT; // 0-255
static constexpr uint32_t LEDC_FREQ = 500;                          // [Hz]

enum MotorChannel { L0 = 0, L1, R0, R1 };

// =====================
// GPIO / PWM setup
// =====================
void setupPinMode() {
  ledc_timer_config_t ledc_timer = {.speed_mode = LEDC_MODE,
                                    .duty_resolution = LEDC_DUTY_RES,
                                    .timer_num = LEDC_TIMER,
                                    .freq_hz = LEDC_FREQ,
                                    .clk_cfg = LEDC_AUTO_CLK};
  ledc_timer_config(&ledc_timer);

  auto config_channel = [](ledc_channel_t ch, gpio_num_t pin) {
    ledc_channel_config_t ledc_ch = {.gpio_num = pin,
                                     .speed_mode = LEDC_MODE,
                                     .channel = ch,
                                     .intr_type = LEDC_INTR_DISABLE,
                                     .timer_sel = LEDC_TIMER,
                                     .duty = 0,
                                     .hpoint = 0};
    ledc_channel_config(&ledc_ch);
  };

  config_channel(LEDC_CHANNEL_0, LEFT_MOTOR_PIN0);
  config_channel(LEDC_CHANNEL_1, LEFT_MOTOR_PIN1);
  config_channel(LEDC_CHANNEL_2, RIGHT_MOTOR_PIN0);
  config_channel(LEDC_CHANNEL_3, RIGHT_MOTOR_PIN1);
}

// =====================
// Motor control helpers
// =====================
static void setMotorDuty(ledc_channel_t ch, uint32_t duty) {
  ledc_set_duty(LEDC_MODE, ch, duty);
  ledc_update_duty(LEDC_MODE, ch);
}

void stop() {
  setMotorDuty(LEDC_CHANNEL_0, 255);
  setMotorDuty(LEDC_CHANNEL_1, 255);
  setMotorDuty(LEDC_CHANNEL_2, 255);
  setMotorDuty(LEDC_CHANNEL_3, 255);
}

static void safeAllLow() {
  setMotorDuty(LEDC_CHANNEL_0, 0);
  setMotorDuty(LEDC_CHANNEL_1, 0);
  setMotorDuty(LEDC_CHANNEL_2, 0);
  setMotorDuty(LEDC_CHANNEL_3, 0);
}

// =====================
// PWM Drive Core
// =====================
static void drivePWM(uint32_t duration_ms, uint8_t target_duty_l,
                     uint8_t target_duty_r, bool is_forward) {
  safeAllLow();

  ledc_channel_t l_ch = is_forward ? LEDC_CHANNEL_0 : LEDC_CHANNEL_1;
  ledc_channel_t r_ch = is_forward ? LEDC_CHANNEL_2 : LEDC_CHANNEL_3;

  const uint8_t start_duty = 120;
  constexpr int RAMP_STEPS = 5;
  constexpr int STEP_MS = 20;

  for (int i = 0; i < RAMP_STEPS; i++) {
    uint8_t d_l = start_duty + (target_duty_l - start_duty) * i / RAMP_STEPS;
    uint8_t d_r = start_duty + (target_duty_r - start_duty) * i / RAMP_STEPS;

    if (target_duty_l < start_duty)
      d_l = target_duty_l;
    if (target_duty_r < start_duty)
      d_r = target_duty_r;

    setMotorDuty(l_ch, d_l);
    setMotorDuty(r_ch, d_r);
    vTaskDelay(pdMS_TO_TICKS(STEP_MS));
  }

  setMotorDuty(l_ch, target_duty_l);
  setMotorDuty(r_ch, target_duty_r);

  uint32_t ramp_total_time = RAMP_STEPS * STEP_MS;
  if (duration_ms > ramp_total_time) {
    vTaskDelay(pdMS_TO_TICKS(duration_ms - ramp_total_time));
  }

  stop();
}

// =====================
// Motion APIs
// =====================
static uint32_t estimateTime_forward(unsigned int distance_cm) {
  const float coefficient = 94;
  const float intercept = 43;
  const float time_ms = (coefficient * distance_cm) + intercept;
  return (time_ms < 0) ? 0 : static_cast<uint32_t>(time_ms);
}

void forward(const unsigned int cm) {
  if (cm == 0)
    return;
  drivePWM(estimateTime_forward(cm), 255, 255, true);
}

static uint32_t estimateTime_backward(unsigned int distance_cm) {
  const float coefficient = 110;
  const float intercept = 130;
  const float time_ms = (coefficient * distance_cm) + intercept;
  return (time_ms < 0) ? 0 : static_cast<uint32_t>(time_ms);
}

void backward(const unsigned int cm) {
  if (cm == 0)
    return;
  drivePWM(estimateTime_backward(cm), 255, 255, false);
}

static constexpr float MAGNIFICATION = 0.4f;
static void twistedDrivePWM(uint32_t total_duration_ms, bool is_forward,
                            bool is_right) {
  float base_max_duty = 255.0f;

  float ratio_left = is_right ? MAGNIFICATION : (1.0f - MAGNIFICATION);
  float ratio_right = is_right ? (1.0f - MAGNIFICATION) : MAGNIFICATION;

  uint8_t duty_l = static_cast<uint8_t>(base_max_duty * ratio_left);
  uint8_t duty_r = static_cast<uint8_t>(base_max_duty * ratio_right);

  safeAllLow();

  ledc_channel_t l_ch = is_right ? LEDC_CHANNEL_0 : LEDC_CHANNEL_1;
  ledc_channel_t r_ch = is_right ? LEDC_CHANNEL_3 : LEDC_CHANNEL_2;

  const uint8_t start_duty = 120;
  constexpr int RAMP_STEPS = 5;
  constexpr int STEP_MS = 20;

  for (int i = 0; i < RAMP_STEPS; i++) {
    uint8_t d_l = start_duty + (duty_l - start_duty) * i / RAMP_STEPS;
    uint8_t d_r = start_duty + (duty_r - start_duty) * i / RAMP_STEPS;

    if (duty_l < start_duty)
      d_l = duty_l;
    if (duty_r < start_duty)
      d_r = duty_r;

    setMotorDuty(l_ch, d_l);
    setMotorDuty(r_ch, d_r);
    vTaskDelay(pdMS_TO_TICKS(STEP_MS));
  }

  setMotorDuty(l_ch, duty_l);
  setMotorDuty(r_ch, duty_r);

  uint32_t ramp_total_time = RAMP_STEPS * STEP_MS;
  if (total_duration_ms > ramp_total_time) {
    vTaskDelay(pdMS_TO_TICKS(total_duration_ms - ramp_total_time));
  }

  stop();
}

void rightRotate() { twistedDrivePWM(2500, true, true); }

void leftRotate() { twistedDrivePWM(2000, true, false); }

// void rightBackwardRotate() { twistedDrivePWM(2000, false, true); }
//
// void leftBackwardRotate() { twistedDrivePWM(2000, false, false); }

} // namespace WheelController
