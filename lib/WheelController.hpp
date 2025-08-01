#ifndef WHEEL_CONTROLLER_H
#define WHEEL_CONTROLLER_H

#include <Arduino.h>

class WheelController {
public:
    // Motor pin definitions
    static constexpr uint8_t LEFT_MOTOR_PIN0  = 5;
    static constexpr uint8_t LEFT_MOTOR_PIN1  = 18;
    static constexpr uint8_t RIGHT_MOTOR_PIN0 = 19;
    static constexpr uint8_t RIGHT_MOTOR_PIN1 = 21;

    // Timing constants
    static constexpr int MILL_SEC_TO_ROTATE_FOR_90 = 3000;
    static constexpr int MILL_SEC_TO_FORWARD_FOR_5CM = 1000;

    /* LeftMotor
     * (Blue Pin, White Pin)
     * (LOW, HIGH) => Reverse
     * (HIGH, LOW) => Forward
     * (HIGH, HIGH) => <STOP>
     * (LOW, LOW) => <STOP>
     */

    /* RightMotor
     * (Blue Pin, Yellow Pin)
     * (LOW, HIGH) => Forward
     * (HIGH, LOW) => Reverse
     * (HIGH, HIGH) => <STOP>
     * (LOW, LOW) => <STOP>
     */

    static void setupPinMode() {
        pinMode(LEFT_MOTOR_PIN0, OUTPUT);
        pinMode(LEFT_MOTOR_PIN1, OUTPUT);
        pinMode(RIGHT_MOTOR_PIN0, OUTPUT);
        pinMode(RIGHT_MOTOR_PIN1, OUTPUT);
    }

    static String getAllPin() {
        String str = "----";
        str.setCharAt(0, digitalRead(LEFT_MOTOR_PIN0) == HIGH ? 'H' : 'L');
        str.setCharAt(1, digitalRead(LEFT_MOTOR_PIN1) == HIGH ? 'H' : 'L');
        str.setCharAt(2, digitalRead(RIGHT_MOTOR_PIN0) == HIGH ? 'H' : 'L');
        str.setCharAt(3, digitalRead(RIGHT_MOTOR_PIN1) == HIGH ? 'H' : 'L');
        return str;
    }

    static void forward(unsigned int cm) {
        if (cm == 0) return;

        setupPinMode();

        // Left motor forward, Right motor forward
        digitalWrite(LEFT_MOTOR_PIN0, HIGH);
        digitalWrite(LEFT_MOTOR_PIN1, LOW);
        digitalWrite(RIGHT_MOTOR_PIN0, HIGH);
        digitalWrite(RIGHT_MOTOR_PIN1, LOW);

        String str = getAllPin();
        Serial.println(str);

        delay(cm * MILL_SEC_TO_FORWARD_FOR_5CM);
        stop();
    }

    static void backward(unsigned int cm) {
        if (cm == 0) return;

        setupPinMode();

        // Left motor backward, Right motor backward
        digitalWrite(LEFT_MOTOR_PIN0, LOW);
        digitalWrite(LEFT_MOTOR_PIN1, HIGH);
        digitalWrite(RIGHT_MOTOR_PIN0, LOW);
        digitalWrite(RIGHT_MOTOR_PIN1, HIGH);

        String str = getAllPin();
        Serial.println(str);

        delay(cm * MILL_SEC_TO_FORWARD_FOR_5CM);
        stop();
    }

    static void rightRotate() {
        setupPinMode();

        // Left motor forward, Right motor backward
        digitalWrite(LEFT_MOTOR_PIN0, HIGH);
        digitalWrite(LEFT_MOTOR_PIN1, LOW);
        digitalWrite(RIGHT_MOTOR_PIN0, LOW);
        digitalWrite(RIGHT_MOTOR_PIN1, HIGH);

        String str = getAllPin();
        Serial.println(str);

        delay(MILL_SEC_TO_ROTATE_FOR_90);
        stop();
    }

    static void leftRotate() {
        setupPinMode();

        // Left motor backward, Right motor forward
        digitalWrite(LEFT_MOTOR_PIN0, LOW);
        digitalWrite(LEFT_MOTOR_PIN1, HIGH);
        digitalWrite(RIGHT_MOTOR_PIN0, HIGH);
        digitalWrite(RIGHT_MOTOR_PIN1, LOW);

        String str = getAllPin();
        Serial.println(str);

        delay(MILL_SEC_TO_ROTATE_FOR_90);
        stop();
    }

    static void stop() {
        setupPinMode();

        // All pins LOW for stop
        digitalWrite(LEFT_MOTOR_PIN0, LOW);
        digitalWrite(LEFT_MOTOR_PIN1, LOW);
        digitalWrite(RIGHT_MOTOR_PIN0, LOW);
        digitalWrite(RIGHT_MOTOR_PIN1, LOW);

        delay(1000);

        String str = getAllPin();
        Serial.println(str);
    }
};

#endif // WHEEL_CONTROLLER_H
