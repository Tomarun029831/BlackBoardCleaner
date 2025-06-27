// === librarise of arduino-mega ===
#include <Arduino.h>
#include <HardwareSerial.h>
#include <WString.h>

// === librarise of esp32 ===
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include <sys/time.h>

// === my librarise ===
#include "lib/KIC.h"
#include "lib/ScheduleGataway.h"

/* === auto script for esp32 ===
// COM10    serial   Unknown
compile script:
arduino-cli compile --fqbn esp32:esp32:esp32wroverkit .\BlackBoardCleaner
*/

/*
upload script:
arduino-cli upload -p COM9 --fqbn esp32:esp32:esp32wroverkit .\BlackBoardCleaner
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

class SerialGateway : public ScheduleGateway{
  public:
    SerialGateway(){}

    int available() override {
      return Serial.available();
    }

  String receiveString() override {
    String result = "";
    while (true) {
      if (Serial.available() <= 0) continue;
      char c = Serial.read();
      if (c == 'K') {  // 'K' から開始と仮定
        result += c;
        break;
      }
    }
    while (true) {
      if (Serial.available() <= 0) continue;
      char c = Serial.read();
      result += c;
      if (c == '/') break;  // 終端文字
    }
    return result;
  }

    void sendString(String str) override {
      Serial.print(str);
    }
};

class WirelessGateway : public ScheduleGateway{ // TODO:
  public:
    WirelessGateway(){}

    int available() override {
      return true;
    }

    String receiveString() override {
      String result = "";
      return result;
    }

    void sendString(String str) override {
    }
};

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

    static void forward(){

    }

    static void backward(){

    }

    static void rightRotate(){

    }

    static void leftRotate(){

    }

    static void stop(){

    }

  private:
    static constexpr uint8_t rightMotorPin0;
    static constexpr uint8_t rightMotorPin1;
    static constexpr uint8_t leftMotorPin0;
    static constexpr uint8_t leftMotorPin1;
}

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

      Serial.println("=== testKICParser START ===");
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
      Serial.print("testKICParser: Test ");
      Serial.println(testPassed ? "passed" : "failed");

      // -- teardown --
      freeSPool(spool);
      Serial.println("=== testKICParser END ===");
    }

    static void runAllTests()
    {
      testKICParser();
    }
};

class MotorPinTestCase{
  public:
    static void testStopSignal()
    {

    }

    static void testForwardSignal()
    {

    }

    static void testBackwardSignal()
    {
    
    }

    static void testRightRotationSignal()
    {

    }

    static void testLeftRotationSignal()
    {

    }

    static void runAllTests()
    {
      testStopSignal();
      delay(3);
      testForwardSignal();
      delay(3);
      testBackwardSignal();
      delay(3);
      testRightRotationSignal();
      delay(3);
      testLeftRotationSignal();
    }
};

class MotorManualTestCase{ // use your eye to test
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
      delay(3);
      testStopMovement();
      delay(3);

      testBackwardMovement();
      delay(3);
      testStopMovement();
      delay(3);

      testRightRotation();
      delay(3);
      testStopMovement();
      delay(3);

      testLeftRotation();
      delay(3);
      testStopMovement();
    }
};

ScheduleGateway* gateway;

static SPool* spool;

void setup() {
  KICProtocolTestCase::runAllTests();
  MotorPinTestCase::runAllTests();
  MotorManualTestCase::runAllTests();
}

void loop() {
}
