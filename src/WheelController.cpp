#include "../lib/WheelController.hpp"
#include "../lib/Timestamp.hpp"
#include <driver/gpio.h>

extern Timestamp machineInternalTimestamp;
extern void delayWithoutCpuStop(unsigned int ms, Timestamp &ts);

constexpr unsigned long one_minute_mills = 60000;

namespace WheelController {

// =====================
// Motor pin definitions
// =====================
static constexpr gpio_num_t LEFT_MOTOR_PIN0 = (gpio_num_t)18;
static constexpr gpio_num_t LEFT_MOTOR_PIN1 = (gpio_num_t)19;
static constexpr gpio_num_t RIGHT_MOTOR_PIN0 = (gpio_num_t)23;
static constexpr gpio_num_t RIGHT_MOTOR_PIN1 = (gpio_num_t)22;

// =====================
// Timing constants
// =====================
static constexpr int MILL_SEC_TO_ROTATE_FOR_90 = 900;

// =====================
// GPIO setup
// =====================
void setupPinMode() {
  gpio_config_t io_conf{};
  io_conf.intr_type = GPIO_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
  io_conf.pin_bit_mask = (1ULL << LEFT_MOTOR_PIN0) | (1ULL << LEFT_MOTOR_PIN1) |
                         (1ULL << RIGHT_MOTOR_PIN0) |
                         (1ULL << RIGHT_MOTOR_PIN1);

  gpio_config(&io_conf);
}

// =====================
// Time estimation
// =====================
static uint32_t estimateTime_forward(unsigned int distance_cm) {
  if (distance_cm == 0)
    return 0;

  float coefficient = 139.11f;
  float intercept = -81.33f;

  float time_ms = (coefficient * ++distance_cm) + intercept;
  return (time_ms < 0) ? 0 : static_cast<uint32_t>(time_ms);
}

static uint32_t estimateTime_backward(unsigned int distance_cm) {
  if (distance_cm == 0)
    return 0;

  float coefficient = 206.27f;
  float intercept = 160.85f;

  float time_ms = (coefficient * distance_cm) + intercept;
  return (time_ms < 0) ? 0 : static_cast<uint32_t>(time_ms);
}

// =====================
// Motor control helpers
// =====================
static void safeStop() {
  gpio_set_level(LEFT_MOTOR_PIN0, 0);
  gpio_set_level(LEFT_MOTOR_PIN1, 0);
  gpio_set_level(RIGHT_MOTOR_PIN0, 0);
  gpio_set_level(RIGHT_MOTOR_PIN1, 0);
}

// =====================
// Motion APIs
// =====================
void forward(unsigned int cm) {
  if (cm == 0)
    return;

  safeStop();

  gpio_set_level(LEFT_MOTOR_PIN0, 1);
  gpio_set_level(RIGHT_MOTOR_PIN0, 1);

  uint32_t delay_ms = estimateTime_forward(cm);
  delayWithoutCpuStop(delay_ms, machineInternalTimestamp);
  stop();
}

void backward(unsigned int cm) {
  if (cm == 0)
    return;

  safeStop();

  gpio_set_level(LEFT_MOTOR_PIN1, 1);
  gpio_set_level(RIGHT_MOTOR_PIN1, 1);

  uint32_t delay_ms = estimateTime_backward(cm);
  delayWithoutCpuStop(delay_ms, machineInternalTimestamp);
  stop();
}

void rightRotate(unsigned int degree) {
  (void)degree; // 現状は 90 度固定

  safeStop();
  gpio_set_level(LEFT_MOTOR_PIN0, 1);

  delayWithoutCpuStop(MILL_SEC_TO_ROTATE_FOR_90, machineInternalTimestamp);
  stop();
}

void leftRotate(unsigned int degree) {
  (void)degree; // 現状は 90 度固定

  safeStop();
  gpio_set_level(RIGHT_MOTOR_PIN0, 1);

  delayWithoutCpuStop(MILL_SEC_TO_ROTATE_FOR_90, machineInternalTimestamp);
  stop();
}

void stop() {
  safeStop();

  // モータドライバ仕様に合わせた停止（ブレーキ）
  gpio_set_level(LEFT_MOTOR_PIN0, 1);
  gpio_set_level(LEFT_MOTOR_PIN1, 1);
  gpio_set_level(RIGHT_MOTOR_PIN0, 1);
  gpio_set_level(RIGHT_MOTOR_PIN1, 1);
}

} // namespace WheelController
