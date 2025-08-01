#include "../test/MotorManualOnFloorTestCase.hpp"
#include "../lib/WheelController.hpp"
#include <Arduino.h>
#include <HardwareSerial.h>

void MotorManualOnFloorTestCase::setup() {
    Serial.println("Setting up motor pins...");
    WheelController::setupPinMode();
    Serial.println("Motor pins setup complete.");
}

void MotorManualOnFloorTestCase::testStopMovement() {
    Serial.println("Testing STOP movement...");
    WheelController::stop();
    Serial.println("STOP test complete.");
}

void MotorManualOnFloorTestCase::testForwardMovement() {
    Serial.println("Testing FORWARD movement (3cm)...");
    WheelController::forward(3);
    Serial.println("FORWARD test complete.");
}

void MotorManualOnFloorTestCase::testBackwardMovement() {
    Serial.println("Testing BACKWARD movement (3cm)...");
    WheelController::backward(3);
    Serial.println("BACKWARD test complete.");
}

void MotorManualOnFloorTestCase::testRightRotation() {
    Serial.println("Testing RIGHT rotation (90 degrees)...");
    WheelController::rightRotate();
    Serial.println("RIGHT rotation test complete.");
}

void MotorManualOnFloorTestCase::testLeftRotation() {
    Serial.println("Testing LEFT rotation (90 degrees)...");
    WheelController::leftRotate();
    Serial.println("LEFT rotation test complete.");
}

void MotorManualOnFloorTestCase::runBasicMovementTest() {
    Serial.println("=== Basic Movement Test START ===");
    
    // Forward movement test
    testForwardMovement();
    testStopMovement();
    delay(2000);
    
    // Repeat forward movement
    testForwardMovement();
    testStopMovement();
    delay(2000);
    
    Serial.println("=== Basic Movement Test END ===");
}

void MotorManualOnFloorTestCase::runAllTests() {
    Serial.println("=== MotorManualOnFloorTest START ===");
    
    // Basic movement test
    runBasicMovementTest();
    
    Serial.println("=== MotorManualOnFloorTest COMPLETE ===");
    Serial.flush();
}
