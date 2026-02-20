// === Arduino ===
#include <HardwareSerial.h>
#include <cmath>

// === modules ===
extern "C" {
  #include "./src/modules/kic_timestamp/kic_timestamp.h"
  #include "./src/modules/kic_parser/kic_parser_specifications.h"
  #include "./src/modules/kic_parser/flyweight_kic_parser.h"
}

// === my libs ===
#include "./lib/WheelController.hpp"
#include "./lib/HTTPBroker.hpp"

/*
let port = "COM10"; let fqbn = "esp32:esp32:esp32";
arduino-cli compile --fqbn $fqbn ~/Documents/BlackBoardCleaner/; arduino-cli upload -p $port --fqbn $fqbn ~/Documents/BlackBoardCleaner/; plink -serial $port -sercfg 115200,8,n,1,N
*/

// === Global States ===
bool isOnceCleaned;
String receiveString = ""; // Parser用に生の文字列を保持
static constexpr int machineWidth = 22;   // cm
static constexpr int machineHeight = 23;  // cm
static bool rightMoveToClean = true;
static bool isPositionedUpper = true;

KIC_Timestamp machineInternalTimestamp;
constexpr unsigned long one_minute_mills = 60000;
unsigned long mills_on_called;

// === Functions ===

/**
 * @brief 掃除実行ロジック
 * modulesのBoardSize構造体（height_cm, width_cm）を使用
 */
static void AutoClean(const BoardSize boardSize) {
  const int heightToMove = boardSize.height_cm - machineHeight;
  int leftWidthToMove = boardSize.width_cm - machineWidth;

  if(heightToMove <= 0 || leftWidthToMove <= 0) {
    Serial.println("BoardSize is too small"); 
    return;
  }

  constexpr int widthToMove = 11; // 11cm
  constexpr int forwardDistanceToSide = 40;
  constexpr int backwardDistanceToSide = 40;
  constexpr int forwardDistanceToFixPosition = 40;
  constexpr int backwardDistanceToFixPosition = 60;

  // 内部の boardSize 参照箇所をすべて .height_cm / .width_cm に置換
  if(rightMoveToClean && isPositionedUpper){
    while(true){
      WheelController::forward(heightToMove);
      isPositionedUpper = false;
      if(leftWidthToMove <= widthToMove) break;
      WheelController::rightRotate(1);
      WheelController::backward(backwardDistanceToSide);
      WheelController::leftRotate(1);
      WheelController::forward(forwardDistanceToFixPosition);
      if(leftWidthToMove <= widthToMove) break;
      leftWidthToMove -= widthToMove;
      WheelController::backward(heightToMove + machineHeight / 2);
      isPositionedUpper = true;
      if(leftWidthToMove <= widthToMove) break;
      WheelController::leftRotate(1);
      WheelController::forward(forwardDistanceToSide);
      WheelController::rightRotate(1);
      WheelController::backward(backwardDistanceToFixPosition);
      if(leftWidthToMove <= widthToMove) break;
      leftWidthToMove -= widthToMove;
      WheelController::forward(heightToMove);
      isPositionedUpper = false;
    }
    rightMoveToClean = false;
  } else if (rightMoveToClean && !isPositionedUpper){
    while(true){
      WheelController::backward(heightToMove);
      isPositionedUpper = true;
      if(leftWidthToMove <= widthToMove) break;
      WheelController::rightRotate(1);
      WheelController::forward(forwardDistanceToSide);
      WheelController::leftRotate(1);
      WheelController::backward(backwardDistanceToFixPosition);
      if(leftWidthToMove <= widthToMove) break;
      leftWidthToMove -= widthToMove;
      WheelController::forward(heightToMove);
      isPositionedUpper = false;
      if(leftWidthToMove <= widthToMove) break;
      WheelController::rightRotate(1);
      WheelController::backward(backwardDistanceToSide);
      WheelController::leftRotate(1);
      WheelController::forward(forwardDistanceToFixPosition);
      if(leftWidthToMove <= widthToMove) break;
      leftWidthToMove -= widthToMove;
      WheelController::backward(heightToMove);
      isPositionedUpper = true;
    }
    rightMoveToClean = false;
  } else if (!rightMoveToClean && isPositionedUpper){
    while(true){
      WheelController::forward(heightToMove);
      isPositionedUpper = false;
      if(leftWidthToMove <= widthToMove) break;
      WheelController::leftRotate(1);
      WheelController::backward(backwardDistanceToSide);
      WheelController::rightRotate(1);
      WheelController::forward(forwardDistanceToFixPosition);
      if(leftWidthToMove <= widthToMove) break;
      leftWidthToMove -= widthToMove;
      WheelController::backward(heightToMove);
      isPositionedUpper = true;
      if(leftWidthToMove <= widthToMove) break;
      WheelController::rightRotate(1);
      WheelController::forward(forwardDistanceToSide);
      WheelController::leftRotate(1);
      WheelController::backward(backwardDistanceToFixPosition);
      if(leftWidthToMove <= widthToMove) break;
      leftWidthToMove -= widthToMove;
      WheelController::forward(heightToMove);
      isPositionedUpper = false;
    }
    rightMoveToClean = true;
  } else if (!rightMoveToClean && !isPositionedUpper){
    while(true){
      WheelController::backward(heightToMove);
      isPositionedUpper = true;
      if(leftWidthToMove <= widthToMove) break;
      WheelController::rightRotate(1);
      WheelController::forward(forwardDistanceToSide);
      WheelController::leftRotate(1);
      WheelController::backward(backwardDistanceToFixPosition);
      if(leftWidthToMove <= widthToMove) break;
      leftWidthToMove -= widthToMove;
      WheelController::forward(heightToMove);
      isPositionedUpper = false;
      if(leftWidthToMove <= widthToMove) break;
      WheelController::rightRotate(1);
      WheelController::backward(backwardDistanceToSide);
      WheelController::leftRotate(1);
      WheelController::forward(forwardDistanceToFixPosition);
      if(leftWidthToMove <= widthToMove) break;
      leftWidthToMove -= widthToMove;
      WheelController::backward(heightToMove);
      isPositionedUpper = true;
    }
    rightMoveToClean = true;
  }
  WheelController::stop();
}

extern "C" void delayWithoutCpuStop(unsigned int ms, KIC_Timestamp &ts){
  unsigned long start_mills = millis();
  while(millis() - start_mills < ms) yield();
  KIC_Timestamp_AddMs(&ts, ms);
}

#define DEBUG_MODE 0

#if DEBUG_MODE
void setup(){
  Serial.begin(115200);
  WheelController::setupPinMode();
  WheelController::stop();
  Serial.println("System Ready (DEBUG)");
}

void loop(){
  // デバッグ用の動作確認など
  WheelController::forward(100);
}
#else
void setup() {
  Serial.begin(115200);
  WheelController::setupPinMode();
  WheelController::stop();

  // 初期データ取得
  receiveString = "KIC:V3;31734;00500050;317351736;/";
  
  if (check_kic_syntax(receiveString.c_str()) != KIC_SYNTAX_CORRECT) {
    Serial.println("KIC Syntax Error");
    ESP.restart();
  }

  // 内部時計をサーバー時刻で初期化
  machineInternalTimestamp = get_kic_timestamp(receiveString.c_str());
  isOnceCleaned = false;
}
#endif

#if !DEBUG_MODE
void loop() {
  // 現在の曜日インデックス取得 (0:Sun - 6:Sat)
  char current_day_char = (char)(machineInternalTimestamp.segments.day + '0');

  // 土曜日（'6'）に新しいデータを取得するロジック
  if (machineInternalTimestamp.segments.day == 6) {
    String newData = HTTPBroker::receiveString();
    if (check_kic_syntax(newData.c_str()) == KIC_SYNTAX_CORRECT) {
      receiveString = newData;
      // 時刻同期
      machineInternalTimestamp = get_kic_timestamp(receiveString.c_str());
    }
  }

  // 1分経過したら「この時間の掃除は完了」フラグを落とす
  if(millis() - mills_on_called >= one_minute_mills) {
    mills_on_called = millis();
    isOnceCleaned = false;
  }

  // スケジュールの確認
  KIC_SchedulePtr daySchedule = find_kic_schedule(receiveString.c_str(), current_day_char);
  
  if (daySchedule != KIC_SCHEDULE_NOT_FOUND && !isOnceCleaned) {
    for (size_t idx = 0; ; idx++) {
      KIC_Timestamp scheduledTime = find_kic_time_in_schedule(daySchedule, idx);
      
      // スケジュール末尾に到達
      if (scheduledTime.segments.is_invalid) break;

      // 時刻一致判定 (AM/PM と 時:分 の一致を確認)
      if (machineInternalTimestamp.segments.hour_min == scheduledTime.segments.hour_min &&
          machineInternalTimestamp.segments.is_PM == scheduledTime.segments.is_PM) {
        
        isOnceCleaned = true;
        mills_on_called = millis();
        
        // 最新のボードサイズを取得して実行
        AutoClean(get_kic_boardsize(receiveString.c_str()));
        break;
      }
    }
  }

  // 100ms待機しつつ内部時計を進める
  delayWithoutCpuStop(100, machineInternalTimestamp);
}
#endif
