#ifndef WHEEL_CONTROLLER_H
#define WHEEL_CONTROLLER_H

#include <driver/gpio.h>

namespace WheelController {
void setupPinMode();
void forward(unsigned int cm);
void backward(unsigned int cm);
void rightForwardRotate();
void leftForwardRotate();
void rightBackwardRotate();
void leftBackwardRotate();
void stop();
} // namespace WheelController

#endif // WHEEL_CONTROLLER_H
