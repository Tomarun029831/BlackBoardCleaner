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
#include "./lib/KIC.hpp"
#include "./lib/WheelController.hpp"
// #include "lib/ScheduleGataway.hpp"
#include "./ENV.hpp"

// === my tests ===
#include "./test/WiFiConnectionTestCase.hpp"
#include "./test/HTTPResponseTestCase.hpp"
#include "./test/MotorPinTestCase.hpp"
#include "./test/MotorManualOnFloorTestCase.hpp"
#include "./test/MotorManualOnWallTestCase.hpp"
#include "./test/KICProtocolTestCase.hpp"


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


void setup() {
  Serial.begin(115200);
  KICProtocolTestCase::runAllTests();
  WheelController::setupPinMode();

  // MotorPinTestCase::runAllTests();
  MotorManualOnFloorTestCase::runAllTests();
  // MotorManualOnWallTestCase::runAllTests();
}

void loop() {
  switch (Serial.read()) {
    case 'w': WheelController::forward(); Serial.println("forward"); break;
    case 's': WheelController::backward(); Serial.println("backward"); break;
    case 'd': WheelController::rightRotate(); Serial.println("rightRotate"); break;
    case 'a': WheelController::leftRotate(); Serial.println("leftRotate"); break;
    case 'q': WheelController::stop(); Serial.println("stop"); break;
    case 'p': {String str = WheelController::getAllPin();Serial.println(str);break;}
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
