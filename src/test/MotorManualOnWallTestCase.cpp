#include "../../test/MotorManualOnWallTestCase.hpp"
#include "../../lib/WheelController.hpp"
#include <Arduino.h>
#include <HardwareSerial.h>

void MotorManualOnWallTestCase::testStopMovement() {
    WheelController::stop();
}

void MotorManualOnWallTestCase::testFarwardMovement() {
    WheelController::forward(5);
}

void MotorManualOnWallTestCase::testBackwardMovement() {
    WheelController::backward(5);
}

void MotorManualOnWallTestCase::testRightRotation() {
    WheelController::rightRotate();
}

void MotorManualOnWallTestCase::testLeftRotation() {
    WheelController::leftRotate();
}

void MotorManualOnWallTestCase::runAllTests() {
    Serial.println("=== MotorManualOnWallTest START ===");

    testFarwardMovement();
    testStopMovement();
    delay(3000);

    testBackwardMovement();
    testStopMovement();
    delay(3000);

    testRightRotation();
    testStopMovement();
    delay(3000);

    testLeftRotation();
    testStopMovement();

    Serial.println("=== MotorManualOnWallTest END ===");
}
