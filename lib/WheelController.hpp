#ifndef WHEEL_CONTROLLER_H
#define WHEEL_CONTROLLER_H

#include <Arduino.h>

namespace WheelController {
    void setupPinMode();
    void forward(unsigned int cm);
    void backward(unsigned int cm);
    void rightRotate(unsigned int dgree); // dgree = 10 (not radian)
    void leftRotate(unsigned int dgree); // dgree = 10 (not radian)
    void stop();
}

#endif // WHEEL_CONTROLLER_H
