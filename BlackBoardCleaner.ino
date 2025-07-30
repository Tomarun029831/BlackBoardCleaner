// === my tests ===
// #include "./test/WiFiConnectionTestCase.hpp"
// #include "./test/HTTPResponseTestCase.hpp"
// #include "./test/MotorPinTestCase.hpp"
// #include "./test/MotorManualOnFloorTestCase.hpp"
// #include "./test/MotorManualOnWallTestCase.hpp"
// #include "./test/KICProtocolTestCase.hpp"

// === core loop ===
// #include "./update.hpp"

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
  awake();

  // === Test ===
  KICProtocolTestCase::runAllTests();
  // WiFiConnectionTestCase::runAllTests();
  // HTTPResponseTestCase::runAllTests();
  // MotorPinTestCase::runAllTests();
  while(1){
    MotorManualOnFloorTestCase::runAllTests();
  }
  // MotorManualOnWallTestCase::runAllTests();
}

void loop() {
  update();
}


// === ESP32用 DCモーター制御コード ===
// DC MOTOR  CW/CCW and SPEED CONTROL (PWM = 490Hz)
//
// int IN1 = 5;
// int IN2 = 18;
// int VR_PIN = A0;
// int VR_VALUE;
// int PWM_VALUE;
// int motorStatus = 0;  // 状態: 0 = 停止, 1 = CCW, 2 = CW
//
// // PWM用設定
// const int pwmFreq = 490;
// const int pwmResolution = 8; // 8bit => 0~255
// const int pwmChannel1 = 0;
// const int pwmChannel2 = 1;
//
// void setup() {
//   // PWMチャンネルのセットアップ
//   ledcSetup(pwmChannel1, pwmFreq, pwmResolution);
//   ledcSetup(pwmChannel2, pwmFreq, pwmResolution);
//
//   // ピンにPWMチャンネルをアタッチ
//   ledcAttachPin(IN1, pwmChannel1);
//   ledcAttachPin(IN2, pwmChannel2);
// }
//
// void PWM_SYORI(){
//   if (motorStatus == 0){  // BREAK
//     ledcWrite(pwmChannel1, 255);
//     ledcWrite(pwmChannel2, 255);
//   }
//   else if (motorStatus == 1){  // CCW
//     ledcWrite(pwmChannel1, PWM_VALUE);
//     ledcWrite(pwmChannel2, 0);
//   }
//   else if (motorStatus == 2){  // CW
//     ledcWrite(pwmChannel1, 0);
//     ledcWrite(pwmChannel2, PWM_VALUE);
//   }
// }
//
// void loop(){
//   motorStatus = (motorStatus + 1) % 3;
//   PWM_SYORI();
//   delay(1000);
// }
