#include "../../test/MotorManualOnFloorTestCase.hpp"
#include "../../lib/WheelController.hpp"
#include <Arduino.h>
#include <HardwareSerial.h>

void MotorManualOnFloorTestCase::setup() {
    WheelController::setupPinMode();
}

void MotorManualOnFloorTestCase::testStopMovement() {
    WheelController::stop();
}

void MotorManualOnFloorTestCase::testFarwardMovement() {
    WheelController::forward();
}

void MotorManualOnFloorTestCase::testBackwardMovement() {
    WheelController::backward();
}

void MotorManualOnFloorTestCase::testRightRotation() {
    WheelController::rightRotate();
}

void MotorManualOnFloorTestCase::testLeftRotation() {
    WheelController::leftRotate();
}

void MotorManualOnFloorTestCase::runAllTests() {
    Serial.println("=== MotorManualOnFloorTest START ===");

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

    Serial.println("=== MotorManualOnFloorTest END ===");
    Serial.flush();
}
