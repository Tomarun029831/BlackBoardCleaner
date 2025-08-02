#ifndef WHEEL_CONTROLLER_H
#define WHEEL_CONTROLLER_H

#include <Arduino.h>

namespace WheelController {
    void setupPinMode();
    String getAllPin();
    void forward(unsigned int cm);
    void backward(unsigned int cm);
    void rightRotate();
    void leftRotate();
    void stop();
}

#endif // WHEEL_CONTROLLER_H
