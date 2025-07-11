#include "../../test/MotorManualOnWallTestCase.hpp"
#include "../../lib/WheelController.hpp"
#include <Arduino.h>
#include <HardwareSerial.h>

void MotorManualOnWallTestCase::testStopMovement() {
    WheelController::stop();
}

void MotorManualOnWallTestCase::testFarwardMovement() {
    WheelController::forward();
}

void MotorManualOnWallTestCase::testBackwardMovement() {
    WheelController::backward();
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
    delay(3000);
    testStopMovement();
    delay(3000);

    testBackwardMovement();
    delay(3000);
    testStopMovement();
    delay(3000);

    testRightRotation();
    delay(3000);
    testStopMovement();
    delay(3000);

    testLeftRotation();
    delay(3000);
    testStopMovement();

    Serial.println("=== MotorManualOnWallTest END ===");
}
