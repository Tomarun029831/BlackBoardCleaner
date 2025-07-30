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
    WheelController::forward(3);
}

void MotorManualOnFloorTestCase::testBackwardMovement() {
    WheelController::backward(3);
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
    // testStopMovement();
    // delay(3000);

    testBackwardMovement();
    // testStopMovement();
    // delay(3000);

    testRightRotation();
    // testStopMovement();
    // delay(3000);

    testLeftRotation();
    // testStopMovement();

    Serial.println("=== MotorManualOnFloorTest END ===");
    Serial.flush();
}
