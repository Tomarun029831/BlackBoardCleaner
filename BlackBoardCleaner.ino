// === my tests ===
#include "./test/WiFiConnectionTestCase.hpp"
#include "./test/HTTPResponseTestCase.hpp"
// #include "./test/MotorPinTestCase.hpp"
#include "./test/MotorManualOnFloorTestCase.hpp"
#include "./test/MotorManualOnWallTestCase.hpp"
#include "./test/KICProtocolTestCase.hpp"
#include "./ENV.hpp"

#include <Arduino.h>
#include <HardwareSerial.h>
#include <WiFi.h>

// === core loop ===
// #include "./lib/WheelController.h"
#include "./update.hpp"



/* === auto script for esp-wroom-32D ===
// COM10    serial   Unknown
compile script:



// Driver
// https://akizukidenshi.com/goodsaffix/AE-DRV8835-S_20210526.pdf

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

/*
BlackBoard:
HEIGHT = 335 cm
WIDTH = 105 cm
*/

/*
MACHINE:
HEIGHT = 20 cm
WIDTH = 15 cm
*/

void setup() {
  Serial.begin(115200);
  // wheel_setup_pin_mode();
  // wheel_stop();
  // awake();

  // === Test ===
  WiFiConnectionTestCase::connect();
  HTTPResponseTestCase::runAllTests();
  // KICProtocolTestCase::runAllTests();
  // MotorPinTestCase::runAllTests();

  // while(1){
    // wheel_forward(3);  // C言語スタイルの関数呼び出し
    // wheel_backward(3);  // C言語スタイルの関数呼び出し
    // wheel_right_rotate();  // C言語スタイルの関数呼び出し
    // wheel_left_rotate();  // C言語スタイルの関数呼び出し
    // MotorManualOnFloorTestCase::runAllTests();
  // }
  // MotorManualOnWallTestCase::runAllTests();
}

//
// void setup() {
//   Serial.begin(115200);
//   WheelController::setupPinMode();
//   WheelController::stop();
//
//   Serial.println("4ピン全16通りテスト開始");
//   Serial.println("leftMotorPin0, leftMotorPin1, rightMotorPin0, rightMotorPin1");
//
//   digitalWrite(WheelController::leftMotorPin0, HIGH);
//   digitalWrite(WheelController::leftMotorPin1, HIGH);
//   digitalWrite(WheelController::rightMotorPin0, HIGH);
//   digitalWrite(WheelController::rightMotorPin1, HIGH);
//
// /*
// パターン 0: 0 0 0 0 L stop, R stop
// パターン 1: 1 0 0 0 L forward, R stop
// パターン 2: 0 1 0 0 L backward, R stop
// パターン 3: 1 1 0 0 L stop, R stop
// パターン 4: 0 0 1 0 L stop, R forward
// パターン 5: 1 0 1 0 L forward, R forward
// パターン 6: 0 1 1 0 L stop, R stop
// パターン 7: 1 1 1 0 L stop, R stop
// パターン 8: 0 0 0 1 L stop, R backward
// パターン 9: 1 0 0 1 L forward, R backward
// パターン 10: 0 1 0 1 L stop, R stop
// パターン 11: 1 1 0 1 L stop, R stop
// パターン 12: 0 0 1 1 L stop, R stop
// パターン 13: 1 0 1 1 L forward, R stop
// パターン 14: 0 1 1 1 L backward, R stop
// パターン 15: 1 1 1 1 L stop, R stop
// */
//
//   // 全16通りの組み合わせをテスト
//   for(int i = 0; i < 16; i++) {
//     // iの各ビットを各ピンに対応させる
//     // bit 0 -> leftMotorPin0
//     // bit 1 -> leftMotorPin1  
//     // bit 2 -> rightMotorPin0
//     // bit 3 -> rightMotorPin1
//
//     bool leftPin0State = (i & 0b0001) != 0;
//     bool leftPin1State = (i & 0b0010) != 0;
//     bool rightPin0State = (i & 0b0100) != 0;
//     bool rightPin1State = (i & 0b1000) != 0;
//
//     digitalWrite(WheelController::leftMotorPin0, leftPin0State ? HIGH : LOW);
//     digitalWrite(WheelController::leftMotorPin1, leftPin1State ? HIGH : LOW);
//     digitalWrite(WheelController::rightMotorPin0, rightPin0State ? HIGH : LOW);
//     digitalWrite(WheelController::rightMotorPin1, rightPin1State ? HIGH : LOW);
//
//     Serial.print("パターン ");
//     Serial.print(i);
//     Serial.print(": ");
//     Serial.print(leftPin0State ? "1" : "0");
//     Serial.print(" ");
//     Serial.print(leftPin1State ? "1" : "0");
//     Serial.print(" ");
//     Serial.print(rightPin0State ? "1" : "0");
//     Serial.print(" ");
//     Serial.println(rightPin1State ? "1" : "0");
//
//     delay(5000);
//   }
//
//   // テスト終了後は全てLOWに
//   digitalWrite(WheelController::leftMotorPin0, LOW);
//   digitalWrite(WheelController::leftMotorPin1, LOW);
//   digitalWrite(WheelController::rightMotorPin0, LOW);
//   digitalWrite(WheelController::rightMotorPin1, LOW);
//
//   Serial.println("テスト完了 - 全ピンLOWに設定");
// }

void loop() {
  // setupで全て完了するため、loopは空
}

