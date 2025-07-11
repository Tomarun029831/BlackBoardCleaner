#include "../../test/MotorPinTestCase.hpp"
#include "../../lib/WheelController.hpp"
#include <Arduino.h>
#include <HardwareSerial.h>

void MotorPinTestCase::testStopSignal() {
    bool testPassed = true;
    WheelController::stop();

    if (digitalRead(WheelController::leftMotorPin0) != HIGH) testPassed = false;
    if (digitalRead(WheelController::leftMotorPin1) != HIGH) testPassed = false;
    if (digitalRead(WheelController::rightMotorPin0) != HIGH) testPassed = false;
    if (digitalRead(WheelController::rightMotorPin1) != HIGH) testPassed = false;

    Serial.print("testStopSignal ");
    Serial.println(testPassed ? "passed" : "failed");
}

void MotorPinTestCase::testForwardSignal() {
    bool testPassed = true;
    WheelController::forward(5);

    if (digitalRead(WheelController::leftMotorPin0) != HIGH) testPassed = false;
    if (digitalRead(WheelController::leftMotorPin1) != LOW) testPassed = false;
    if (digitalRead(WheelController::rightMotorPin0) != HIGH) testPassed = false;
    if (digitalRead(WheelController::rightMotorPin1) != LOW) testPassed = false;

    Serial.print("testForwardSignal ");
    Serial.println(testPassed ? "passed" : "failed");
}

void MotorPinTestCase::testBackwardSignal() {
    bool testPassed = true;
    WheelController::backward(5);

    if (digitalRead(WheelController::leftMotorPin0) != LOW) testPassed = false;
    if (digitalRead(WheelController::leftMotorPin1) != HIGH) testPassed = false;
    if (digitalRead(WheelController::rightMotorPin0) != LOW) testPassed = false;
    if (digitalRead(WheelController::rightMotorPin1) != HIGH) testPassed = false;

    Serial.print("testBackwardSignal ");
    Serial.println(testPassed ? "passed" : "failed");
}

void MotorPinTestCase::testRightRotationSignal() {
    bool testPassed = true;
    WheelController::rightRotate();

    if (digitalRead(WheelController::leftMotorPin0) != HIGH) testPassed = false;
    if (digitalRead(WheelController::leftMotorPin1) != LOW) testPassed = false;
    if (digitalRead(WheelController::rightMotorPin0) != LOW) testPassed = false;
    if (digitalRead(WheelController::rightMotorPin1) != HIGH) testPassed = false;

    Serial.print("testRightRotationSignal ");
    Serial.println(testPassed ? "passed" : "failed");
}

void MotorPinTestCase::testLeftRotationSignal() {
    bool testPassed = true;
    WheelController::leftRotate();

    if (digitalRead(WheelController::leftMotorPin0) != LOW) testPassed = false;
    if (digitalRead(WheelController::leftMotorPin1) != HIGH) testPassed = false;
    if (digitalRead(WheelController::rightMotorPin0) != HIGH) testPassed = false;
    if (digitalRead(WheelController::rightMotorPin1) != LOW) testPassed = false;

    Serial.print("testLeftRotationSignal ");
    Serial.println(testPassed ? "passed" : "failed");
}

void MotorPinTestCase::runAllTests() {
    Serial.println("=== MotorPinTest START ===");
    testStopSignal();
    testForwardSignal();
    testBackwardSignal();
    testRightRotationSignal();
    testLeftRotationSignal();
    Serial.println("=== MotorPinTest END ===");
    Serial.flush();
}
