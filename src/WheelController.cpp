#include "../lib/WheelController.hpp"
#include "../lib/Timestamp.hpp"
#include <driver/gpio.h>

extern Timestamp machineInternalTimestamp;
constexpr unsigned long one_minute_mills = 60000; // 60000

namespace WheelController {
// Motor pin definitions
static constexpr uint8_t LEFT_MOTOR_PIN0 = 18;  // AIN1
static constexpr uint8_t LEFT_MOTOR_PIN1 = 19;  // AIN2
static constexpr uint8_t RIGHT_MOTOR_PIN0 = 23; // BIN1
static constexpr uint8_t RIGHT_MOTOR_PIN1 = 22; // BIN2

/*


1100 -> X


*/

// Timing constants
static constexpr int MILL_SEC_TO_ROTATE_FOR_90 = 900;

// forward on terakado white board
static uint32_t estimateTime_forward(unsigned int distance_cm) {
  if (distance_cm == 0)
    return 0;
  float coefficient = 139.11;
  float intercept = -81.33;

  // white board
  // float time_ms = (coefficient * ++distance_cm) + intercept;
  // 4I board
  // float time_ms = (coefficient * distance_cm) + intercept;
  float time_ms = (coefficient * ++distance_cm) + intercept;
  // float time_ms = (coefficient * (distance_cm - 1)) + intercept;

  if (time_ms < 0)
    return 0;

  return time_ms;
}

// backward on terakado white board
static uint32_t estimateTime_backward(unsigned int distance_cm) {
  if (distance_cm == 0)
    return 0;
  float coefficient = 206.27;
  float intercept = 160.85;

  float time_ms = (coefficient * distance_cm) + intercept;
  // float time_ms = (coefficient * --distance_cm) + intercept;
  // float time_ms = (coefficient * (distance_cm + 2)) + intercept;

  if (time_ms < 0)
    return 0;

  return time_ms;
}

static void safeStop() {
  setupPinMode();
  digitalWrite(LEFT_MOTOR_PIN0, LOW);
  digitalWrite(LEFT_MOTOR_PIN1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN0, LOW);
  digitalWrite(RIGHT_MOTOR_PIN1, LOW);
}

void setupPinMode() {
  pinMode(LEFT_MOTOR_PIN0, OUTPUT);
  pinMode(LEFT_MOTOR_PIN1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN0, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN1, OUTPUT);
}

void forward(unsigned int cm) {
  if (cm == 0)
    return;

  safeStop();
  // digitalWrite(LEFT_MOTOR_PIN0, HIGH);
  // digitalWrite(RIGHT_MOTOR_PIN0, HIGH);

  gpio_set_level((gpio_num_t)LEFT_MOTOR_PIN0, 1);
  gpio_set_level((gpio_num_t)RIGHT_MOTOR_PIN0, 1);

  uint32_t delay_ms = estimateTime_forward(cm);
  delay(delay_ms);
  machineInternalTimestamp.hour_minute += delay_ms / one_minute_mills;
  stop();
}

void backward(unsigned int cm) {
  if (cm == 0)
    return;

  safeStop();
  digitalWrite(LEFT_MOTOR_PIN1, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN1, HIGH);

  uint32_t delay_ms = estimateTime_backward(cm);
  delay(delay_ms);
  machineInternalTimestamp.hour_minute += delay_ms / one_minute_mills;
  stop();
}

void rightRotate(unsigned int dgree) {
  safeStop();
  digitalWrite(LEFT_MOTOR_PIN0, HIGH);
  // digitalWrite(LEFT_MOTOR_PIN1, LOW);
  // digitalWrite(RIGHT_MOTOR_PIN0, LOW);
  // digitalWrite(RIGHT_MOTOR_PIN1, HIGH);

  uint32_t delay_ms = MILL_SEC_TO_ROTATE_FOR_90;
  delay(delay_ms);
  machineInternalTimestamp.hour_minute += delay_ms / one_minute_mills;
  stop();
}

void leftRotate(unsigned int dgree) {
  safeStop();
  // digitalWrite(LEFT_MOTOR_PIN0, LOW);
  // digitalWrite(LEFT_MOTOR_PIN1, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN0, HIGH);
  // digitalWrite(RIGHT_MOTOR_PIN1, LOW);

  uint32_t delay_ms = MILL_SEC_TO_ROTATE_FOR_90;
  delay(delay_ms);
  machineInternalTimestamp.hour_minute += delay_ms / one_minute_mills;
  stop();
}

void stop() {
  safeStop();
  digitalWrite(LEFT_MOTOR_PIN0, HIGH);
  digitalWrite(LEFT_MOTOR_PIN1, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN0, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
}
} // namespace WheelController
