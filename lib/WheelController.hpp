#ifndef _WHEELCONTROLLER_
#define _WHEELCONTROLLER_

#include <Arduino.h>

class WheelController {
public:
    static String getAllPin();
    static void setupPinMode();

    static void forward();
    static void backward();
    static void rightRotate();
    static void leftRotate();
    static void stop();

    static constexpr uint8_t rightMotorPin0 = 18; // D18 BIN1
    static constexpr uint8_t rightMotorPin1 = 5;  // D5  BIN2
    static constexpr uint8_t leftMotorPin0  = 21; // D21 AIN1
    static constexpr uint8_t leftMotorPin1  = 19; // D19 AIN2
};

#endif // !_WHEELCONTROLLER_
