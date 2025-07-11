#include "../../lib/WheelController.hpp"
#include <Arduino.h>

static constexpr int MillSecToRotateFor90 = 1000;
static constexpr int MillSecToForwardFor5cm = 1000;

String WheelController::getAllPin() {
    String str = "----"; // 4文字確保
    str.setCharAt(0, digitalRead(leftMotorPin0)  == HIGH ? 'H' : 'L');
    str.setCharAt(1, digitalRead(leftMotorPin1)  == HIGH ? 'H' : 'L');
    str.setCharAt(2, digitalRead(rightMotorPin0) == HIGH ? 'H' : 'L');
    str.setCharAt(3, digitalRead(rightMotorPin1) == HIGH ? 'H' : 'L');
    return str;
}

void WheelController::setupPinMode() {
    pinMode(leftMotorPin0, OUTPUT);
    pinMode(leftMotorPin1, OUTPUT);
    pinMode(rightMotorPin0, OUTPUT);
    pinMode(rightMotorPin1, OUTPUT);
}

void WheelController::forward(uint cm) {
    if(cm == 0)return;

    digitalWrite(leftMotorPin0, HIGH);
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(rightMotorPin0, HIGH);
    digitalWrite(rightMotorPin1, LOW);
    delay(cm * MillSecToForwardFor5cm);
    WheelController::stop();
}

void WheelController::backward(uint cm) {
    if(cm == 0)return;

    digitalWrite(leftMotorPin0, LOW);
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(rightMotorPin0, LOW);
    digitalWrite(rightMotorPin1, HIGH);
    delay(cm * MillSecToForwardFor5cm);
    WheelController::stop();
}

void WheelController::rightRotate() {
    digitalWrite(leftMotorPin0, HIGH);
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(rightMotorPin0, LOW);
    digitalWrite(rightMotorPin1, HIGH);
    delay(MillSecToRotateFor90);
    WheelController::stop();
}

void WheelController::leftRotate() {
    digitalWrite(leftMotorPin0, LOW);
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(rightMotorPin0, HIGH);
    digitalWrite(rightMotorPin1, LOW);
    delay(MillSecToRotateFor90);
    WheelController::stop();
}

void WheelController::stop() {
    digitalWrite(leftMotorPin0, HIGH);
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(rightMotorPin0, HIGH);
    digitalWrite(rightMotorPin1, HIGH);
}
