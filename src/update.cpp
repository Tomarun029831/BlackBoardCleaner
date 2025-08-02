// === librarise of arduino-mega ===
#include <Arduino.h>
#include <HardwareSerial.h>
#include <WString.h>

// === librarise of esp32 ===
#include <HTTPClient.h>
#include <WiFi.h>
#include <Wire.h>
#include <sys/time.h>
#include <time.h>

// === core ===
#include "../update.hpp"

// === my librarise ===
#include "../ENV.hpp"
#include "../lib/KIC.hpp"
#include "../lib/ScheduleGateway.hpp"
#include "../lib/WheelController.hpp"

constexpr uint WIDTH_BLACKBOARD = 335; /* cm */
constexpr uint HIGH_BLACKBOARD = 105;  /* cm */

static ScheduleGateway *gateways[2] = {new SerialGateway(),
                                       new WirelessGateway()};

void startCleaning(uint high_cm, uint width_cm) {
  constexpr uint BODY_HEIGHT_CM = 20;
  uint segments = high_cm / (2 * BODY_HEIGHT_CM);

  for (uint i = 0; i < segments; ++i) {
    WheelController::forward(width_cm);

    WheelController::rightRotate();
    WheelController::forward(BODY_HEIGHT_CM);
    WheelController::rightRotate();

    WheelController::forward(width_cm);

    WheelController::leftRotate();
    WheelController::forward(BODY_HEIGHT_CM);
    WheelController::leftRotate();
  }

  WheelController::stop();
}

void awake() {
  WheelController::setupPinMode();
  WheelController::stop();

  for (ScheduleGateway* gateway : gateways) {
    gateway->setup();
  }
}

void update() {
  switch (Serial.read()) {
    case 'W':
        WheelController::forward(1);
        Serial.println("forward");
        break;
    case 'S':
        WheelController::backward(1);
        Serial.println("backward");
        break;
    case 'D':
        WheelController::rightRotate();
        Serial.println("rightRotate");
        break;
    case 'A':
        WheelController::leftRotate();
        Serial.println("leftRotate");
        break;
    case 'Q':
        WheelController::stop();
        Serial.println("stop");
        break;
    case 'E':
        startCleaning(HIGH_BLACKBOARD, WIDTH_BLACKBOARD);
        Serial.println("auto cleaning");
        break;
    case 'P': {
        String str = WheelController::getAllPin();
        Serial.println(str);
        break;
    }
  }

  Serial.flush();

  // Serial.println("Out of ForEach");
  //
  // for (ScheduleGateway* gateway : gateways) {
  //   Serial.println("In ForEach");
  //   if (!gateway->available()) continue;
  //   Serial.println("In ForEach: over the gard");
  //
  //   String ret = gateway->receiveString();
  //   if (ret.length() > 0) {
  //     Serial.print("Received from gateway: ");
  //     Serial.println(ret);
  //   }
  // }
}
