#ifndef _WHEELCONTROLLER_
#define _WHEELCONTROLLER_

#include <Arduino.h>

class WheelController {
public:
    static String getAllPin();
    static void setupPinMode();

    static void forward(uint cm);
    static void backward(uint cm);
    static void rightRotate();
    static void leftRotate();
    static void stop();

/* LeftMotor
 * (Blue Pin, White Pin)
 * (LOW, HIGH) => Reverse
 * (HIGH, LOW) => Forward
 * (HIGH, HIGH) => <STOP>
 * (LOW, LOW) => <STOP>
 */
    static constexpr uint8_t leftMotorPin0  = 5;
    static constexpr uint8_t leftMotorPin1  = 18;

/* RightMotor
 * (Blue Pin, Yellow Pin)
 * (LOW, HIGH) => Forward
 * (HIGH, LOW) => Reverse
 * (HIGH, HIGH) => <STOP>
 * (LOW, LOW) => <STOP>
 */
    static constexpr uint8_t rightMotorPin0 = 19;
    static constexpr uint8_t rightMotorPin1 = 21;

};

#endif // !_WHEELCONTROLLER_
