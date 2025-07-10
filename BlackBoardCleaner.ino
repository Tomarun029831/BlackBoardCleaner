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
#include "ENV.h"

/* === auto script for esp-wroom-32D ===
// COM10    serial   Unknown
compile script:

ESP32-WROOM-DA Module                                                   esp32:esp32:esp32da
ESP32 Wrover Kit (all versions)                                         esp32:esp32:esp32wroverkit
ESP32 Wrover Module                                                     esp32:esp32:esp32wrover
ESP32 Dev Module                                                        esp32:esp32:esp32

let sketch = "./BlackBoardCleaner"

let boards = [
  "esp32:esp32:esp32da",
  "esp32:esp32:esp32wroverkit",
  "esp32:esp32:esp32wrover",
  "esp32:esp32:esp32"
]

for board in $boards {
  print $"=== Checking ($board) ==="
  let result = (arduino-cli compile --fqbn $board $sketch | complete)
  if $result.exit_code == 0 {
    print $"✅ Success: ($board)"
  } else {
    print $"❌ Failed:  ($board)"
    print $"    stderr: ($result.stderr | lines | get 0?)"
  }
}

*/

/*
upload script:
arduino-cli upload -p COM9 --fqbn esp32:esp32:esp32 .\BlackBoardCleaner;
*/

/*
auto compile, upload and monitor:

let fqbn = "esp32:esp32:esp32"
let port = "COM5"

arduino-cli compile --fqbn $fqbn .\BlackBoardCleaner;arduino-cli upload -p $port --fqbn $fqbn .\BlackBoardCleaner; arduino-cli monitor -p $port --config baudrate=115200;
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

char ** boardGrid;

void clearSerialBuffer() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}

class WheelHandler{
  public:
    static String getAllPin() {
      String str = "----"; // 初期化して4文字確保
      str.setCharAt(0, (digitalRead(WheelHandler::leftMotorPin0) == HIGH)  ? 'H' : 'L');
      str.setCharAt(1, (digitalRead(WheelHandler::leftMotorPin1) == HIGH)  ? 'H' : 'L');
      str.setCharAt(2, (digitalRead(WheelHandler::rightMotorPin0) == HIGH) ? 'H' : 'L');
      str.setCharAt(3, (digitalRead(WheelHandler::rightMotorPin1) == HIGH) ? 'H' : 'L');
      return str;
    }

    static void setupPinMode(){
      pinMode(WheelHandler::leftMotorPin0, OUTPUT);
      pinMode(WheelHandler::leftMotorPin1, OUTPUT);
      pinMode(WheelHandler::rightMotorPin0, OUTPUT);
      pinMode(WheelHandler::rightMotorPin1, OUTPUT);
    }

    /*

    MODE xIN1 xIN2 xOUT1 xOUT2 動作
    0 0 1 L H 逆転
    0 1 0 H L 正転  TODO:
    0 1 1 L L ブレーキ

    */

    static void forward(){
      digitalWrite(WheelHandler::leftMotorPin0, HIGH);
      digitalWrite(WheelHandler::leftMotorPin1, LOW);
      digitalWrite(WheelHandler::rightMotorPin0, HIGH);
      digitalWrite(WheelHandler::rightMotorPin1, LOW);
    }

    static void backward(){
      digitalWrite(WheelHandler::leftMotorPin0, LOW);
      digitalWrite(WheelHandler::leftMotorPin1, HIGH);
      digitalWrite(WheelHandler::rightMotorPin0, LOW);
      digitalWrite(WheelHandler::rightMotorPin1, HIGH);
    }

    static void rightRotate(){ // TODO: args: float dgree
      digitalWrite(WheelHandler::leftMotorPin0, HIGH);
      digitalWrite(WheelHandler::leftMotorPin1, LOW);
      digitalWrite(WheelHandler::rightMotorPin0, LOW);
      digitalWrite(WheelHandler::rightMotorPin1, HIGH);
    }

    static void leftRotate(){ // TODO: args: float dgree
      digitalWrite(WheelHandler::leftMotorPin0, LOW);
      digitalWrite(WheelHandler::leftMotorPin1, HIGH);
      digitalWrite(WheelHandler::rightMotorPin0, HIGH);
      digitalWrite(WheelHandler::rightMotorPin1, LOW);
    }

    static void stop(){
      digitalWrite(WheelHandler::leftMotorPin0, HIGH);
      digitalWrite(WheelHandler::leftMotorPin1, HIGH);
      digitalWrite(WheelHandler::rightMotorPin0, HIGH);
      digitalWrite(WheelHandler::rightMotorPin1, HIGH);
    }

    // pin list: D5, D18, D19, D21
    static constexpr uint8_t rightMotorPin0 = 18; // D18 BIN1
    static constexpr uint8_t rightMotorPin1 = 5; // D5 BIN2
    static constexpr uint8_t leftMotorPin0 = 21; // D21 AIN1
    static constexpr uint8_t leftMotorPin1 = 19; // D19 AIN2
};

// === TEST CASE ===
class KICProtocolTestCase
{
  public:
    static void testKICParser() {
      // -- setup --
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
      Serial.flush();
    }
};

class MotorPinTestCase{
  public:
    static void testStopSignal()
    {
      bool testPassed = true;
      WheelHandler::stop();

      if(digitalRead(WheelHandler::leftMotorPin0) != HIGH) testPassed = false;
      if(digitalRead(WheelHandler::leftMotorPin1) != HIGH) testPassed = false;
      if(digitalRead(WheelHandler::rightMotorPin0) != HIGH) testPassed = false;
      if(digitalRead(WheelHandler::rightMotorPin1) != HIGH) testPassed = false;

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
      Serial.flush();
    }
};

class MotorManualOnFloorTestCase{
  public:
    static void setup(){
      WheelHandler::setupPinMode();
    }

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
      Serial.flush();
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
};

class WiFiConnectionTestCase
{
  public:
    static void connect()
    {
      Serial.println("[WiFi] Connecting...");
      WiFi.begin(CONFIG::SSID, CONFIG::PASSWORD);

      unsigned long startAttemptTime = millis();
      const unsigned long timeout = INTMAX_MAX;

      while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout)
      {
        delay(500);
        Serial.print(".");
      }

      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n[WiFi] Connected!");
        Serial.print("[WiFi] IP address: ");
        Serial.println(WiFi.localIP());
      } else {
        Serial.println("\n[WiFi] Connection failed.");
      }
    }

    static void runAllTests()
    {
      Serial.println("=== WiFiConnectionTest START ===");
      connect();
      Serial.println("=== WiFiConnectionTest END ===\n");
      Serial.flush();
    }
};

class HTTPResponseTestCase
{
  public:
    static void runAllTests()
    {
      Serial.println("=== HTTPResponseTest START ===");

      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[HTTP] WiFi not connected. Test aborted.");
        return;
      }

      HTTPClient http;
      http.begin(CONFIG::APIENDPOINT);

      int httpCode = http.GET();
      if (httpCode > 0) {
        Serial.printf("[HTTP] Response code: %d\n", httpCode);
        String payload = http.getString();
        Serial.println("[HTTP] Payload:");
        Serial.println(payload);
      } else {
        Serial.printf("[HTTP] Request failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
      Serial.println("=== HTTPResponseTest END ===\n");
      Serial.flush();
    }
};

void setup() {
  Serial.begin(115200);
  KICProtocolTestCase::runAllTests();
  WheelHandler::setupPinMode();

  // MotorPinTestCase::runAllTests();
  MotorManualOnFloorTestCase::runAllTests();
  // MotorManualOnWallTestCase::runAllTests();
}

void loop() {
  switch (Serial.read()) {
    case 'w': WheelHandler::forward(); Serial.println("forward"); break;
    case 's': WheelHandler::backward(); Serial.println("backward"); break;
    case 'd': WheelHandler::rightRotate(); Serial.println("rightRotate"); break;
    case 'a': WheelHandler::leftRotate(); Serial.println("leftRotate"); break;
    case 'q': WheelHandler::stop(); Serial.println("stop"); break;
    case 'p': {String str = WheelHandler::getAllPin();Serial.println(str);break;}
  }

  Serial.flush();
}

// const int watchPins[] = {
//   0, 1, 2, 3, 4, 5,
//   12, 13, 14, 15, 16, 17, 18, 19,
//   21, 22, 23,
//   25, 26, 27,
//   32, 33,
//   34, 35, 36, 39  // 入力専用（readはOK）
// };

// const int numPins = sizeof(watchPins) / sizeof(watchPins[0]);
// int lastState[40];  // 多めに確保（indexはピン番号）

// void setup() {
//   Serial.begin(115200);
//   // WiFiConnectionTestCase::runAllTests();
//   // HTTPResponseTestCase::runAllTests();
//
//   Serial.flush();
//   Serial.println("In setup");
//   Serial.flush();
//
//   for (int i = 2; i < numPins; i++) {
//     int pin = watchPins[i];
//     pinMode(pin, INPUT);
//     lastState[pin] = digitalRead(pin);
//     Serial.printf("GPIO%d :", i);
//     Serial.println(lastState[pin]);
//     Serial.flush();
//   }
// }
//
// void loop() {
//   for (int i = 2; i < numPins; i++) {
//     int pin = watchPins[i];
//     int state = digitalRead(pin);
//     if (state == lastState[pin]) continue;
//     Serial.printf("GPIO%d changed to %d\n", pin, state);
//     Serial.flush();
//     lastState[pin] = state;
//   }
//   delay(100);
// }
