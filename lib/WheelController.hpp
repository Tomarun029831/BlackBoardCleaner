#ifndef WHEEL_CONTROLLER_H
#define WHEEL_CONTROLLER_H

#include <Arduino.h>
static constexpr gpio_num_t PIN_TO_WEAKUP_IC = (gpio_num_t)27;

namespace WheelController {
void setupPinMode();
void forward(unsigned int cm);
void backward(unsigned int cm);
void rightRotate(unsigned int dgree); // dgree = 10 (not radian)
void leftRotate(unsigned int dgree);  // dgree = 10 (not radian)
void stop();
} // namespace WheelController

#endif // WHEEL_CONTROLLER_H
