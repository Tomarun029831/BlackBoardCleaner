#include "../../lib/WheelController.hpp"
#include <Arduino.h>

static constexpr int MillSecToRotateFor90 = 3000;
static constexpr int MillSecToForwardFor5cm = 1000;
static constexpr uint pwm_channel0 = 0;
static constexpr uint pwm_channel1 = 1;

String WheelController::getAllPin() {
    String str = "----"; // 4文字確保
    // PWMを使っているチャネルのデューティ比を取得
    int duty_left  = ledcRead(pwm_channel0);  // leftMotorPin1のPWMデューティ
    int duty_right = ledcRead(pwm_channel1);  // rightMotorPin1のPWMデューティ

    str.setCharAt(0, digitalRead(leftMotorPin0)  == HIGH ? 'H' : 'L');
    str.setCharAt(1, digitalRead(leftMotorPin1) == HIGH ? 'H' : 'L');
    // str.setCharAt(1, duty_left  > 0 ? 'H' : 'L');
    str.setCharAt(2, digitalRead(rightMotorPin0) == HIGH ? 'H' : 'L');
    str.setCharAt(3, digitalRead(rightMotorPin1) == HIGH ? 'H' : 'L');
    // str.setCharAt(3, duty_right > 0 ? 'H' : 'L');

    return str;
}

void WheelController::setupPinMode() {
    pinMode(leftMotorPin0, OUTPUT);
    pinMode(leftMotorPin1, OUTPUT);
    pinMode(rightMotorPin0, OUTPUT);
    pinMode(rightMotorPin1, OUTPUT);
    String str = WheelController::getAllPin();
    Serial.println(str);
}

// void WheelController::setupPinMode() {
//     // PWMチャンネル設定（周波数1kHz, 分解能8bit）
//     ledcSetup(pwm_channel0, 1000, 8);  // チャンネル0 ← leftMotorPin1用
//     ledcSetup(pwm_channel1, 1000, 8);  // チャンネル1 ← rightMotorPin1用
//
//     // ピンとチャンネルをバインド
//     ledcAttachPin(leftMotorPin1, 0);
//     ledcAttachPin(rightMotorPin1, 1);
//
//     // 他のピンは普通に設定
//     pinMode(leftMotorPin0, OUTPUT);
//     pinMode(rightMotorPin0, OUTPUT);
//
//     // PWM duty 初期化（0%）
//     ledcWrite(pwm_channel0, 0);
//     ledcWrite(pwm_channel1, 0);
//
//     String str = WheelController::getAllPin();
//     Serial.println(str);
// }

void WheelController::forward(uint cm) {
    if(cm == 0)return;

    digitalWrite(leftMotorPin0, HIGH);
    // ledcWrite(pwm_channel0, 0);
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(rightMotorPin0, HIGH);
    digitalWrite(rightMotorPin1, LOW);
    // ledcWrite(pwm_channel1, 0);
    String str = WheelController::getAllPin();
    Serial.println(str);
    delay(cm * MillSecToForwardFor5cm);
    WheelController::stop();
}

void WheelController::backward(uint cm) {
    if(cm == 0)return;

    digitalWrite(leftMotorPin0, LOW);
    // digitalWrite(leftMotorPin1, HIGH);
    ledcWrite(pwm_channel0, 100);
    digitalWrite(rightMotorPin0, LOW);
    // digitalWrite(rightMotorPin1, HIGH);
    ledcWrite(pwm_channel1, 100);
    String str = WheelController::getAllPin();
    Serial.println(str);
    delay(cm * MillSecToForwardFor5cm);
    WheelController::stop();
}

void WheelController::rightRotate() {
    digitalWrite(leftMotorPin0, HIGH);
    digitalWrite(leftMotorPin1, LOW);
    // ledcWrite(pwm_channel0, 0);
    digitalWrite(rightMotorPin0, LOW);
    digitalWrite(rightMotorPin1, HIGH);
    // ledcWrite(pwm_channel1, 100);
    String str = WheelController::getAllPin();
    Serial.println(str);
    delay(MillSecToRotateFor90);
    WheelController::stop();
}

void WheelController::leftRotate() {
    digitalWrite(leftMotorPin0, LOW);
    digitalWrite(leftMotorPin1, HIGH);
    // ledcWrite(pwm_channel0, 100);
    digitalWrite(rightMotorPin0, HIGH);
    digitalWrite(rightMotorPin1, LOW);
    // ledcWrite(pwm_channel1, 0);

    String str = WheelController::getAllPin();
    Serial.println(str);
    delay(MillSecToRotateFor90);
    WheelController::stop();
}

void WheelController::stop() {
    digitalWrite(leftMotorPin0, HIGH);
    digitalWrite(leftMotorPin1, HIGH);
    // ledcWrite(pwm_channel0, 100);
    digitalWrite(rightMotorPin0, HIGH);
    // ledcWrite(pwm_channel1, 100);
    digitalWrite(rightMotorPin1, HIGH);

    // digitalWrite(leftMotorPin0, LOW);
    // digitalWrite(leftMotorPin1, LOW);
    // digitalWrite(rightMotorPin0, LOW);
    // digitalWrite(rightMotorPin1, LOW);
    String str = WheelController::getAllPin();
    Serial.println(str);
}
