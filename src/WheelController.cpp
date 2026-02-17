#include "../lib/WheelController.hpp"
#include "../lib/Timestamp.hpp"
#include <driver/gpio.h>
#include <driver/ledc.h>

extern Timestamp machineInternalTimestamp;
extern void delayWithoutCpuStop(unsigned int ms, Timestamp &ts);

namespace WheelController {

// =====================
// Motor pin definitions
// =====================
static constexpr gpio_num_t LEFT_MOTOR_PIN0 = (gpio_num_t)33;
static constexpr gpio_num_t LEFT_MOTOR_PIN1 = (gpio_num_t)32;
static constexpr gpio_num_t RIGHT_MOTOR_PIN0 = (gpio_num_t)26;
static constexpr gpio_num_t RIGHT_MOTOR_PIN1 = (gpio_num_t)25;

// =====================
// PWM (LEDC) Constants
// =====================
static constexpr ledc_mode_t LEDC_MODE = LEDC_LOW_SPEED_MODE;
static constexpr ledc_timer_t LEDC_TIMER = LEDC_TIMER_0;
static constexpr ledc_timer_bit_t LEDC_RESOLUTION = LEDC_TIMER_10_BIT; // 0-1023
static constexpr uint32_t LEDC_FREQ = 5000;

// 画像の通り、交互に動かすための設定
// DRIVE_DUTY を 512(50%) 未満にすることで、物理的な重なりをゼロにします
static constexpr uint32_t DRIVE_DUTY = 480;
static constexpr uint32_t HPOINT_OFFSET = 512; // 右モーターの開始点を半分ずらす
static constexpr uint32_t STOP_DUTY = 1023;    // ブレーキ（全ピンHigh）

// =====================
// Time estimation
// =====================
uint32_t estimateTime_forward(unsigned int distance_cm) {
  if (distance_cm == 0)
    return 0;
  float coefficient = 139.11f;
  float intercept = -81.33f;
  float time_ms = (coefficient * (distance_cm + 1)) + intercept;
  return (time_ms < 0) ? 0 : static_cast<uint32_t>(time_ms);
}

uint32_t estimateTime_backward(unsigned int distance_cm) {
  if (distance_cm == 0)
    return 0;
  float coefficient = 206.27f;
  float intercept = 160.85f;
  float time_ms = (coefficient * distance_cm) + intercept;
  return (time_ms < 0) ? 0 : static_cast<uint32_t>(time_ms);
}

// =====================
// PWM Control Helpers
// =====================
static void setDuty(ledc_channel_t ch, uint32_t duty) {
  ledc_set_duty(LEDC_MODE, ch, duty);
  ledc_update_duty(LEDC_MODE, ch);
}

static void allLow() {
  for (int i = 0; i < 4; i++) {
    setDuty((ledc_channel_t)i, 0);
  }
}

// =====================
// GPIO & PWM Setup
// =====================
void setupPinMode() {
  // 1. タイマー設定
  ledc_timer_config_t timer_conf = {};
  timer_conf.speed_mode = LEDC_MODE;
  timer_conf.duty_resolution = LEDC_RESOLUTION;
  timer_conf.timer_num = LEDC_TIMER;
  timer_conf.freq_hz = LEDC_FREQ;
  timer_conf.clk_cfg = LEDC_AUTO_CLK;
  ledc_timer_config(&timer_conf);

  // 2. 各ピンの設定
  struct PinCfg {
    gpio_num_t pin;
    ledc_channel_t ch;
    uint32_t hpoint;
  };
  PinCfg configs[] = {
      {LEFT_MOTOR_PIN0, LEDC_CHANNEL_0, 0},              // 左0
      {LEFT_MOTOR_PIN1, LEDC_CHANNEL_1, 0},              // 左1
      {RIGHT_MOTOR_PIN0, LEDC_CHANNEL_2, HPOINT_OFFSET}, // 右0 (位相ずらし)
      {RIGHT_MOTOR_PIN1, LEDC_CHANNEL_3, HPOINT_OFFSET}  // 右1 (位相ずらし)
  };

  for (const auto &c : configs) {
    ledc_channel_config_t ch_conf = {};
    ch_conf.gpio_num = c.pin;
    ch_conf.speed_mode = LEDC_MODE;
    ch_conf.channel = c.ch;
    ch_conf.intr_type = LEDC_INTR_DISABLE;
    ch_conf.timer_sel = LEDC_TIMER;
    ch_conf.duty = 0;
    ch_conf.hpoint = c.hpoint;
    ledc_channel_config(&ch_conf);
  }
}

// =====================
// Motion APIs
// =====================
void stop() {
  setDuty(LEDC_CHANNEL_0, STOP_DUTY);
  setDuty(LEDC_CHANNEL_1, STOP_DUTY);
  setDuty(LEDC_CHANNEL_2, STOP_DUTY);
  setDuty(LEDC_CHANNEL_3, STOP_DUTY);
}

void forward(unsigned int cm) {
  if (cm == 0)
    return;
  allLow();
  setDuty(LEDC_CHANNEL_0, DRIVE_DUTY); // Left Forward
  setDuty(LEDC_CHANNEL_2,
          DRIVE_DUTY); // Right Forward (hpointにより自動でずれる)

  uint32_t delay_ms = estimateTime_forward(cm);
  delayWithoutCpuStop(delay_ms, machineInternalTimestamp);
  stop();
}

void backward(unsigned int cm) {
  if (cm == 0)
    return;
  allLow();
  setDuty(LEDC_CHANNEL_1, DRIVE_DUTY); // Left Backward
  setDuty(LEDC_CHANNEL_3,
          DRIVE_DUTY); // Right Backward (hpointにより自動でずれる)

  uint32_t delay_ms = estimateTime_backward(cm);
  delayWithoutCpuStop(delay_ms, machineInternalTimestamp);
  stop();
}

void rightRotate(unsigned int degree) {
  allLow();
  setDuty(LEDC_CHANNEL_0, DRIVE_DUTY);
  delayWithoutCpuStop(900, machineInternalTimestamp);
  stop();
}

void leftRotate(unsigned int degree) {
  allLow();
  setDuty(LEDC_CHANNEL_2, DRIVE_DUTY);
  delayWithoutCpuStop(900, machineInternalTimestamp);
  stop();
}

} // namespace WheelController
