#include "../lib/WheelController.hpp"

namespace WheelController {
    // Motor pin definitions
    static constexpr uint8_t LEFT_MOTOR_PIN0  = 5;  // AIN1
    static constexpr uint8_t LEFT_MOTOR_PIN1  = 18; // AIN2
    static constexpr uint8_t RIGHT_MOTOR_PIN0 = 19; // BIN1
    static constexpr uint8_t RIGHT_MOTOR_PIN1 = 21; // BIN2

    // Timing constants
    static constexpr int MILL_SEC_TO_ROTATE_FOR_90 = 3000;
    static constexpr int MILL_SEC_TO_FORWARD_FOR_5CM = 1000;

    void setupPinMode() {
        pinMode(LEFT_MOTOR_PIN0, OUTPUT);
        pinMode(LEFT_MOTOR_PIN1, OUTPUT);
        pinMode(RIGHT_MOTOR_PIN0, OUTPUT);
        pinMode(RIGHT_MOTOR_PIN1, OUTPUT);
    }

    String getAllPin() {
        String str = "----";
        str.setCharAt(0, digitalRead(LEFT_MOTOR_PIN0) == HIGH ? 'H' : 'L');
        str.setCharAt(1, digitalRead(LEFT_MOTOR_PIN1) == HIGH ? 'H' : 'L');
        str.setCharAt(2, digitalRead(RIGHT_MOTOR_PIN0) == HIGH ? 'H' : 'L');
        str.setCharAt(3, digitalRead(RIGHT_MOTOR_PIN1) == HIGH ? 'H' : 'L');
        return str;
    }

    void forward(unsigned int cm) {
        if (cm == 0) return;

        safeStop();
        setupPinMode();

        digitalWrite(LEFT_MOTOR_PIN0, HIGH);
        digitalWrite(LEFT_MOTOR_PIN1, LOW);
        digitalWrite(RIGHT_MOTOR_PIN0, HIGH);
        digitalWrite(RIGHT_MOTOR_PIN1, LOW);

        Serial.println(getAllPin());

        delay(cm * MILL_SEC_TO_FORWARD_FOR_5CM);
        stop();
    }

    void backward(unsigned int cm) {
        if (cm == 0) return;

        safeStop();
        setupPinMode();

        digitalWrite(LEFT_MOTOR_PIN0, LOW);
        digitalWrite(LEFT_MOTOR_PIN1, HIGH);
        digitalWrite(RIGHT_MOTOR_PIN0, LOW);
        digitalWrite(RIGHT_MOTOR_PIN1, HIGH);

        Serial.println(getAllPin());

        delay(cm * MILL_SEC_TO_FORWARD_FOR_5CM);
        stop();
    }

    void rightRotate() {
        safeStop();
        setupPinMode();

        digitalWrite(LEFT_MOTOR_PIN0, HIGH);
        digitalWrite(LEFT_MOTOR_PIN1, LOW);
        digitalWrite(RIGHT_MOTOR_PIN0, LOW);
        digitalWrite(RIGHT_MOTOR_PIN1, HIGH);

        Serial.println(getAllPin());

        delay(MILL_SEC_TO_ROTATE_FOR_90);
        stop();
    }

    void leftRotate() {
        safeStop();
        setupPinMode();

        digitalWrite(LEFT_MOTOR_PIN0, LOW);
        digitalWrite(LEFT_MOTOR_PIN1, HIGH);
        digitalWrite(RIGHT_MOTOR_PIN0, HIGH);
        digitalWrite(RIGHT_MOTOR_PIN1, LOW);

        Serial.println(getAllPin());

        delay(MILL_SEC_TO_ROTATE_FOR_90);
        stop();
    }

    void stop() {
        safeStop();

        delay(100);

        digitalWrite(LEFT_MOTOR_PIN0, HIGH);
        digitalWrite(LEFT_MOTOR_PIN1, HIGH);
        digitalWrite(RIGHT_MOTOR_PIN0, HIGH);
        digitalWrite(RIGHT_MOTOR_PIN1, HIGH);

        Serial.println(getAllPin());
    }

    static void safeStop() {
        setupPinMode();

        digitalWrite(LEFT_MOTOR_PIN0, LOW);
        digitalWrite(LEFT_MOTOR_PIN1, LOW);
        digitalWrite(RIGHT_MOTOR_PIN0, LOW);
        digitalWrite(RIGHT_MOTOR_PIN1, LOW);

        delay(100); // bridge preservation
    }
}
