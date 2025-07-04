// === librarise of arduino-mega ===
#include <Arduino.h>
#include <HardwareSerial.h>
#include <WString.h>

// === librarise of esp32 ===
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include <sys/time.h>
#include <Wire.h>

// === my librarise ===
#include "lib/KIC.h"
// #include "lib/ScheduleGataway.h"

/* === auto script for esp-wroom-32D ===
// COM10    serial   Unknown
compile script:
arduino-cli compile --fqbn esp32:esp32:esp32devkit .\BlackBoardCleaner
*/

/*
upload script:


esp32da

arduino-cli upload -p COM9 --fqbn esp32:esp32:esp32devkit .\BlackBoardCleaner
*/

/*
auto compile, upload and monitor:
arduino-cli compile --fqbn esp32:esp32:esp32devkit .\BlackBoardCleaner;
arduino-cli upload -p COM5 --fqbn esp32:esp32:esp32devkit .\BlackBoardCleaner;arduino-cli monitor -p COM5 --config baudrate=115200;
*/

/* === auto script for arduino-mega ===
// COM9    serial   Serial Port (USB) Arduino Mega or Mega 2560 arduino:avr:mega arduino:avr
compile script:
arduino-cli compile --fqbn arduino:avr:mega .\BlackBoardCleaner
*/

/*
upload script:
arduino-cli upload -p COM9 --fqbn arduino:avr:mega .\BlackBoardCleaner
*/

/*
auto compile, upload and monitor:
arduino-cli compile --fqbn arduino:avr:mega .\BlackBoardCleaner && arduino-cli upload -p COM9 --fqbn arduino:avr:mega .\BlackBoardCleaner && 
arduino-cli monitor -p COM9 --config baudrate=9600
*/

void clearSerialBuffer() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}

class WheelHandler{
  public:
    static String getAllPin(){
      String str = "";
      str.setCharAt(0, leftMotorPin0 ? 'H' : 'L');
      str.setCharAt(1, leftMotorPin1 ? 'H' : 'L');
      str.setCharAt(2, rightMotorPin0 ? 'H' : 'L');
      str.setCharAt(3, rightMotorPin1 ? 'H' : 'L');

      return str;
    }

    static void setupPinMode(){
      pinMode(leftMotorPin0, OUTPUT);
      pinMode(leftMotorPin1, OUTPUT);
      pinMode(rightMotorPin0, OUTPUT);
      pinMode(rightMotorPin1, OUTPUT);
    }

    static void forward(){
      digitalWrite(leftMotorPin0, HIGH);
      digitalWrite(leftMotorPin1, LOW);
      digitalWrite(rightMotorPin0, HIGH);
      digitalWrite(rightMotorPin1, LOW);
    }

    static void backward(){
      digitalWrite(leftMotorPin0, LOW);
      digitalWrite(leftMotorPin1, HIGH);
      digitalWrite(rightMotorPin0, LOW);
      digitalWrite(rightMotorPin1, HIGH);
    }

    static void rightRotate(){ // TODO: args: float dgree
      digitalWrite(leftMotorPin0, HIGH);
      digitalWrite(leftMotorPin1, LOW);
      digitalWrite(rightMotorPin0, LOW);
      digitalWrite(rightMotorPin1, HIGH);
    }

    static void leftRotate(){ // TODO: args: float dgree
      digitalWrite(leftMotorPin0, LOW);
      digitalWrite(leftMotorPin1, HIGH);
      digitalWrite(rightMotorPin0, HIGH);
      digitalWrite(rightMotorPin1, LOW);
    }

    static void stop(){
      digitalWrite(leftMotorPin0, LOW);
      digitalWrite(leftMotorPin1, LOW);
      digitalWrite(rightMotorPin0, LOW);
      digitalWrite(rightMotorPin1, LOW);
    }

    // pin list: D5, D18, D19, D21
    static constexpr uint8_t rightMotorPin0 = 5; // D5
    static constexpr uint8_t rightMotorPin1 = 18; // D18
    static constexpr uint8_t leftMotorPin0 = 19; // D19
    static constexpr uint8_t leftMotorPin1 = 21; // D21
};

// === TEST CASE ===
class KICProtocolTestCase
{
  public:
    static void testKICParser() {
      // -- setup --
      Serial.begin(9600);
      const String inputStr = "KIC:V1;1437;1090010001130;208000900;8;9;/";
      const char expectedCmds[]   = {'1', '2', '8', '9', '/'};
      const int  expectedCounts[] = {3,    2,   0,   0,   0};
      const int  expectedHours[][3] = {
        {900, 1000, 1130},
        {800, 900, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
      };
      const int numSchedules = sizeof(expectedCmds) / sizeof(expectedCmds[0]);

      Serial.print("Received String: ");
      Serial.println(inputStr);

      bool testPassed = true;
      SPool* spool = KIC::convertToSPool(inputStr);

      // -- test --
      if (spool->count != numSchedules) {
        Serial.print("ERROR: schedule count mismatch. expected ");
        Serial.print(numSchedules);
        Serial.print(", actual ");
        Serial.println(spool->count);
        testPassed = false;
      }

      for (unsigned int i = 0; i < spool->count && i < numSchedules; i++) {
        Schedule* schedule = *(spool->schedules + i);

        if (schedule->cmd != expectedCmds[i]) {
          Serial.print("ERROR: cmd mismatch at schedule ");
          Serial.print(i);
          Serial.print(". expected '");
          Serial.print(expectedCmds[i]);
          Serial.print("', got '");
          Serial.print(schedule->cmd);
          Serial.println("'");
          testPassed = false;
        }

        if (schedule->count != expectedCounts[i]) {
          Serial.print("ERROR: hours count mismatch at schedule ");
          Serial.print(i);
          Serial.print(". expected ");
          Serial.print(expectedCounts[i]);
          Serial.print(", got ");
          Serial.println(schedule->count);
          testPassed = false;
        }

        for (unsigned int j = 0; j < schedule->count; j++) {
          if (schedule->hours[j] != expectedHours[i][j]) {
            Serial.print("ERROR: hour mismatch at schedule ");
            Serial.print(i);
            Serial.print(", index ");
            Serial.print(j);
            Serial.print(". expected ");
            Serial.print(expectedHours[i][j]);
            Serial.print(", got ");
            Serial.println(schedule->hours[j]);
            testPassed = false;
          }
        }
      }

      // -- result --
      Serial.print("testKICParser ");
      Serial.println(testPassed ? "passed" : "failed");

      // -- teardown --
      freeSPool(spool);
    }

    static void runAllTests()
    {
      Serial.println("=== KICProtocolTest START ===");
      testKICParser();
      Serial.println("=== KICProtocolTest END ===");
    }
};

class MotorPinTestCase{
  public:
    static void testStopSignal()
    {
      bool testPassed = true;
      WheelHandler::stop();

      if(digitalRead(WheelHandler::leftMotorPin0) != LOW) testPassed = false;
      if(digitalRead(WheelHandler::leftMotorPin1) != LOW) testPassed = false;
      if(digitalRead(WheelHandler::rightMotorPin0) != LOW) testPassed = false;
      if(digitalRead(WheelHandler::rightMotorPin1) != LOW) testPassed = false;

      Serial.print("testStopSignal ");
      Serial.println(testPassed ? "passed" : "failed");
    }

    static void testForwardSignal()
    {
      bool testPassed = true;
      WheelHandler::forward();

      if(digitalRead(WheelHandler::leftMotorPin0) != HIGH) testPassed = false;
      if(digitalRead(WheelHandler::leftMotorPin1) != LOW) testPassed = false;
      if(digitalRead(WheelHandler::rightMotorPin0) != HIGH) testPassed = false;
      if(digitalRead(WheelHandler::rightMotorPin1) != LOW) testPassed = false;

      Serial.print("testForwardSignal ");
      Serial.println(testPassed ? "passed" : "failed");
    }

    static void testBackwardSignal()
    {
      bool testPassed = true;
      WheelHandler::backward();

      if(digitalRead(WheelHandler::leftMotorPin0) != LOW) testPassed = false;
      if(digitalRead(WheelHandler::leftMotorPin1) != HIGH) testPassed = false;
      if(digitalRead(WheelHandler::rightMotorPin0) != LOW) testPassed = false;
      if(digitalRead(WheelHandler::rightMotorPin1) != HIGH) testPassed = false;

      Serial.print("testBackwardSignal ");
      Serial.println(testPassed ? "passed" : "failed");
    }

    static void testRightRotationSignal()
    {
      bool testPassed = true;
      WheelHandler::rightRotate();

      if(digitalRead(WheelHandler::leftMotorPin0) != HIGH) testPassed = false;
      if(digitalRead(WheelHandler::leftMotorPin1) != LOW) testPassed = false;
      if(digitalRead(WheelHandler::rightMotorPin0) != LOW) testPassed = false;
      if(digitalRead(WheelHandler::rightMotorPin1) != HIGH) testPassed = false;

      Serial.print("testBackwardSignal ");
      Serial.println(testPassed ? "passed" : "failed");
    }

    static void testLeftRotationSignal()
    {
      bool testPassed = true;
      WheelHandler::leftRotate();

      if(digitalRead(WheelHandler::leftMotorPin0) != LOW) testPassed = false;
      if(digitalRead(WheelHandler::leftMotorPin1) != HIGH) testPassed = false;
      if(digitalRead(WheelHandler::rightMotorPin0) != HIGH) testPassed = false;
      if(digitalRead(WheelHandler::rightMotorPin1) != LOW) testPassed = false;

      Serial.print("testBackwardSignal ");
      Serial.println(testPassed ? "passed" : "failed");
    }

    static void runAllTests()
    {
      Serial.println("=== MotorPinTest START ===");
      testStopSignal();
      delay(3000);
      testForwardSignal();
      delay(3000);
      testBackwardSignal();
      delay(3000);
      testRightRotationSignal();
      delay(3000);
      testLeftRotationSignal();
      Serial.println("=== MotorPinTest END ===");
    }
};

class MotorManualOnFloorTestCase{
  public:
    static void testStopMovement()
    {
      WheelHandler::stop();
    }

    static void testFarwardMovement()
    {
      WheelHandler::forward();
    }

    static void testBackwardMovement()
    {
      WheelHandler::backward();
    }

    static void testRightRotation()
    {
      WheelHandler::rightRotate();
    }

    static void testLeftRotation()
    {
      WheelHandler::leftRotate();
    }

    static void runAllTests()
    {
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
    }
};

class MotorManualOnWallTestCase{
  public:
    static void testStopMovement()
    {
      WheelHandler::stop();
    }

    static void testFarwardMovement()
    {
      WheelHandler::forward();
    }

    static void testBackwardMovement()
    {
      WheelHandler::backward();
    }

    static void testRightRotation()
    {
      WheelHandler::rightRotate();
    }

    static void testLeftRotation()
    {
      WheelHandler::leftRotate();
    }

    static void runAllTests()
    {
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
    }
};

class WiFiConnectionTestCase
{
  public:
    static void connect()
    {
    // HTTPClient http;
      // ScheduleGateway* gateway = new WirelessGateway();
      // gateway->setup();
    }
};
//
// void setup() {
//   KICProtocolTestCase::runAllTests();
//   // MotorPinTestCase::runAllTests();
//   MotorManualOnFloorTestCase::runAllTests();
//   // MotorManualOnWallTestCase::runAllTests();
// }
//
// void loop() {
// }

const int watchPins[] = {
  0, 1, 2, 3, 4, 5,
  12, 13, 14, 15, 16, 17, 18, 19,
  21, 22, 23,
  25, 26, 27,
  32, 33,
  34, 35, 36, 39  // 入力専用（readはOK）
};

const int numPins = sizeof(watchPins) / sizeof(watchPins[0]);
int lastState[40];  // 多めに確保（indexはピン番号）

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < numPins; i++) {
    int pin = watchPins[i];
    pinMode(pin, INPUT);
    lastState[pin] = digitalRead(pin);
  }
}

void loop() {
  for (int i = 0; i < numPins; i++) {
    int pin = watchPins[i];
    int state = digitalRead(pin);
    if (state != lastState[pin]) {
      Serial.printf("GPIO%d changed to %d\n", pin, state);
      lastState[pin] = state;
    }
  }
  delay(100);
}
