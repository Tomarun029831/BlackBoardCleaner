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

    // OK: 800, 500, 250, 150, SPEED(120) == SPEED(110) == SPEED(100) == SPEED(90) == SPEED(60) > SPEED(20) == SPEED(5):: == UNCONTROLLABLE
    static constexpr unsigned int SWITCH_INTERVAL = 5;

    static void safeStop() {
        setupPinMode();
        digitalWrite(LEFT_MOTOR_PIN0, LOW);
        digitalWrite(LEFT_MOTOR_PIN1, LOW);
        digitalWrite(RIGHT_MOTOR_PIN0, LOW);
        digitalWrite(RIGHT_MOTOR_PIN1, LOW);
        delay(100); // bridge preservation
    }

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

    static void pseudoMotorControl(bool leftForward, bool leftBackward, 
                                 bool rightForward, bool rightBackward, 
                                 unsigned long duration) {
        unsigned long startTime = millis();
        unsigned long lastSwitchTime = 0;
        bool isLeftMotorActive = true;

        safeStop();
        setupPinMode();
        Serial.println("Starting pseudo motor control");

        while (millis() - startTime < duration) {
            unsigned long currentTime = millis();

            if (currentTime - lastSwitchTime >= SWITCH_INTERVAL) {
                lastSwitchTime = currentTime;
                isLeftMotorActive = !isLeftMotorActive;

                if (isLeftMotorActive) {
                    if (leftForward) {
                        digitalWrite(LEFT_MOTOR_PIN0, HIGH);
                        digitalWrite(LEFT_MOTOR_PIN1, LOW);
                    } else if (leftBackward) {
                        digitalWrite(LEFT_MOTOR_PIN0, LOW);
                        digitalWrite(LEFT_MOTOR_PIN1, HIGH);
                    } else {
                        digitalWrite(LEFT_MOTOR_PIN0, LOW);
                        digitalWrite(LEFT_MOTOR_PIN1, LOW);
                    }
                    digitalWrite(RIGHT_MOTOR_PIN0, LOW);
                    digitalWrite(RIGHT_MOTOR_PIN1, LOW);
                } else {
                    if (rightForward) {
                        digitalWrite(RIGHT_MOTOR_PIN0, HIGH);
                        digitalWrite(RIGHT_MOTOR_PIN1, LOW);
                    } else if (rightBackward) {
                        digitalWrite(RIGHT_MOTOR_PIN0, LOW);
                        digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
                    } else {
                        digitalWrite(RIGHT_MOTOR_PIN0, LOW);
                        digitalWrite(RIGHT_MOTOR_PIN1, LOW);
                    }
                    // 左モーターは停止
                    digitalWrite(LEFT_MOTOR_PIN0, LOW);
                    digitalWrite(LEFT_MOTOR_PIN1, LOW);
                }
                
                Serial.println(getAllPin());
            }
            
            // 少し待機（CPUを他の処理に譲る）
            delayMicroseconds(100);
        }
    }
    
    void forward(unsigned int cm) {
        if (cm == 0) return;

        // 疑似同時制御で前進
        pseudoMotorControl(true, false, true, false, cm * MILL_SEC_TO_FORWARD_FOR_5CM);
        stop();
    }

    void backward(unsigned int cm) {
        if (cm == 0) return;

        // 疑似同時制御で後退
        pseudoMotorControl(false, true, false, true, cm * MILL_SEC_TO_FORWARD_FOR_5CM);
        stop();
    }

    void rightRotate() {
        // 疑似同時制御で右回転（左前進、右後退）
        pseudoMotorControl(true, false, false, true, MILL_SEC_TO_ROTATE_FOR_90);
        stop();
    }

    void leftRotate() {
        // 疑似同時制御で左回転（左後退、右前進）
        pseudoMotorControl(false, true, true, false, MILL_SEC_TO_ROTATE_FOR_90);
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
        Serial.println("Movement completed and stopped");
    }
}
