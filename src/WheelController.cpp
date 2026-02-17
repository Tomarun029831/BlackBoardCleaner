#include "../lib/WheelController.hpp"
#include "../lib/Timestamp.hpp"
#include <driver/gpio.h>

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
// Timing constants
// =====================
// 1サイクルあたりの時間(ms)。1sで30回切り替える場合は約33msですが、
// ここでは以前の安定値 20ms (50Hz相当) を基準にしています。
static constexpr int MUX_STEP_MS = 33;
static constexpr int MILL_SEC_TO_ROTATE_FOR_90 = 900;

// =====================
// Time estimation (エラー回避のため、呼び出し元より前に配置)
// =====================
static uint32_t estimateTime_forward(unsigned int distance_cm) {
  if (distance_cm == 0)
    return 0;
  float coefficient = 139.11f;
  float intercept = -81.33f;
  float time_ms = (coefficient * (distance_cm + 1)) + intercept;
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
void stop() {
  // 全ピンHighでブレーキ（ドライバの仕様に準拠）
  gpio_set_level(LEFT_MOTOR_PIN0, 1);
  gpio_set_level(LEFT_MOTOR_PIN1, 1);
  gpio_set_level(RIGHT_MOTOR_PIN0, 1);
  gpio_set_level(RIGHT_MOTOR_PIN1, 1);
}

static void safeAllLow() {
  gpio_set_level(LEFT_MOTOR_PIN0, 0);
  gpio_set_level(LEFT_MOTOR_PIN1, 0);
  gpio_set_level(RIGHT_MOTOR_PIN0, 0);
  gpio_set_level(RIGHT_MOTOR_PIN1, 0);
}

// 擬似的に両輪を動かすコアロジック (Time Division)
static void multiplexDrive(uint32_t total_duration_ms, bool forward_direction) {
  uint32_t elapsed = 0;
  gpio_num_t left_pin = forward_direction ? LEFT_MOTOR_PIN0 : LEFT_MOTOR_PIN1;
  gpio_num_t right_pin =
      forward_direction ? RIGHT_MOTOR_PIN0 : RIGHT_MOTOR_PIN1;

  while (elapsed < total_duration_ms) {
    // --- 左モーターのみ駆動 ---
    safeAllLow();
    gpio_set_level(left_pin, 1);
    delayWithoutCpuStop(MUX_STEP_MS / 2, machineInternalTimestamp);

    // --- 右モーターのみ駆動 ---
    safeAllLow();
    gpio_set_level(right_pin, 1);
    delayWithoutCpuStop(MUX_STEP_MS / 2, machineInternalTimestamp);

    elapsed += MUX_STEP_MS;
  }
}

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
// Motion APIs
// =====================
void forward(unsigned int cm) {
  if (cm == 0)
    return;
  uint32_t delay_ms = estimateTime_forward(cm);
  multiplexDrive(delay_ms, true);
  stop();
}

void backward(unsigned int cm) {
  if (cm == 0)
    return;
  uint32_t delay_ms = estimateTime_backward(cm);
  multiplexDrive(delay_ms, false);
  stop();
}

void rightRotate(unsigned int degree) {
  (void)degree;
  safeAllLow();
  gpio_set_level(LEFT_MOTOR_PIN0, 1);
  delayWithoutCpuStop(MILL_SEC_TO_ROTATE_FOR_90, machineInternalTimestamp);
  stop();
}

void leftRotate(unsigned int degree) {
  (void)degree;
  safeAllLow();
  gpio_set_level(RIGHT_MOTOR_PIN0, 1);
  delayWithoutCpuStop(MILL_SEC_TO_ROTATE_FOR_90, machineInternalTimestamp);
  stop();
}

} // namespace WheelController
