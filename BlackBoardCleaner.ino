// === Arduino  ===
#include <HardwareSerial.h>
#include <cmath>

// === my libs ===
#include "./lib/WheelController.hpp"
#include "./lib/KICCollection.hpp"
// #include "./lib/sj.h"
#include "./lib/HTTPBroker.hpp"
#include "./lib/Timestamp.hpp"

/*
arduino-cli compile --fqbn $fqbn ~/Documents/BlackBoardCleaner/; arduino-cli upload -p $port --fqbn $fqbn ~/Documents/BlackBoardCleaner/; plink -serial $port -sercfg 115200,8,n,1,N
*/

/*
HEIGHT: 5 * 15cm + 10cm = 85 cm
WIDTH: 8 * 15cm = 120 cm
*/

/*
BlackBoard:
HEIGHT = 335 cm
WIDTH = 105 cm

MACHINE:
HEIGHT = 15 + 8 = 23 cm
WIDTH = 15 + 6.7 = 21.7 -> 22 cm
*/

bool isOnceCleaned;
static KICCollection::KICData kicData;
static constexpr int machineWidth = 22;   // cm
static constexpr int machineHeight = 23;  // cm
static bool rightMoveToClean = true;
static bool isPositionedUpper = true;

static void AutoClean(const KICCollection::Board boardSize) {
  const int heightToMove = boardSize.height - machineHeight;
  int leftWidthToMove = boardSize.width - machineWidth;

  if(heightToMove <= 0 || leftWidthToMove <= 0) {Serial.println("BoardSize is too small"); return;}

  constexpr int widthToMove = 11; // 11cm
  constexpr int forwardDistanceToSide = 40;
  constexpr int backwardDistanceToSide = 40;
  constexpr int forwardDistanceToFixPosition = 40;
  constexpr int backwardDistanceToFixPosition = 60; // 40

  if(rightMoveToClean && isPositionedUpper){
    while(true){
      // forward to clean
      WheelController::forward(heightToMove);
      isPositionedUpper = false;

      // slide to side at under position
      if(leftWidthToMove <= widthToMove) return;
      WheelController::rightRotate(1);
      WheelController::backward(backwardDistanceToSide);
      WheelController::leftRotate(1);
      WheelController::forward(forwardDistanceToFixPosition);
      if(leftWidthToMove <= widthToMove) return;
      leftWidthToMove -= widthToMove;
      // backward to clean
      WheelController::backward(heightToMove + machineHeight / 2);
      isPositionedUpper = true;

      // slide to side at upper position
      if(leftWidthToMove <= widthToMove) return;
      WheelController::leftRotate(1);
      WheelController::forward(forwardDistanceToSide);
      WheelController::rightRotate(1);
      WheelController::backward(backwardDistanceToFixPosition);
      if(leftWidthToMove <= widthToMove) return;
      leftWidthToMove -= widthToMove;
      // forward to clean
      WheelController::forward(heightToMove);
      isPositionedUpper = false;
    }
    rightMoveToClean = false;
  } else if (rightMoveToClean && !isPositionedUpper){
    while(true){
      // forward to clean
      WheelController::backward(heightToMove);
      isPositionedUpper = true;

      // slide to side at upper position
      if(leftWidthToMove <= widthToMove) return;
      WheelController::rightRotate(1);
      WheelController::forward(forwardDistanceToSide);
      WheelController::leftRotate(1);
      WheelController::backward(backwardDistanceToFixPosition);
      if(leftWidthToMove <= widthToMove) return;
      leftWidthToMove -= widthToMove;
      // forward to clean
      WheelController::forward(heightToMove);
      isPositionedUpper = false;

      // slide to side at under position
      if(leftWidthToMove <= widthToMove) return;
      WheelController::rightRotate(1);
      WheelController::backward(backwardDistanceToSide);
      WheelController::leftRotate(1);
      WheelController::forward(forwardDistanceToFixPosition);
      if(leftWidthToMove <= widthToMove) return;
      leftWidthToMove -= widthToMove;
      // backward to clean
      WheelController::backward(heightToMove);
      isPositionedUpper = true;
    }
    rightMoveToClean = false;
  } else if (!rightMoveToClean && isPositionedUpper){
    while(true){
      // forward to clean
      WheelController::forward(heightToMove);
      isPositionedUpper = false;

      // slide to side at under position
      if(leftWidthToMove <= widthToMove) return;
      WheelController::leftRotate(1);
      WheelController::backward(backwardDistanceToSide);
      WheelController::rightRotate(1);
      WheelController::forward(forwardDistanceToFixPosition);
      if(leftWidthToMove <= widthToMove) return;
      leftWidthToMove -= widthToMove;
      // backward to clean
      WheelController::backward(heightToMove);
      isPositionedUpper = true;

      // slide to side at upper position
      if(leftWidthToMove <= widthToMove) return;
      WheelController::rightRotate(1);
      WheelController::forward(forwardDistanceToSide);
      WheelController::leftRotate(1);
      WheelController::backward(backwardDistanceToFixPosition);
      if(leftWidthToMove <= widthToMove) return;
      leftWidthToMove -= widthToMove;
      // forward tp clean
      WheelController::forward(heightToMove);
      isPositionedUpper = false;
    }
    rightMoveToClean = true;
  } else if (!rightMoveToClean && !isPositionedUpper){
    while(true){
      // forward to clean
      WheelController::backward(heightToMove);
      isPositionedUpper = true;

      // slide to side at upper position
      if(leftWidthToMove <= widthToMove) return;
      WheelController::rightRotate(1);
      WheelController::forward(forwardDistanceToSide);
      WheelController::leftRotate(1);
      WheelController::backward(backwardDistanceToFixPosition);
      if(leftWidthToMove <= widthToMove) return;
      leftWidthToMove -= widthToMove;
      // backward to clean
      WheelController::forward(heightToMove);
      isPositionedUpper = false;

      // slide to side at under position
      if(leftWidthToMove <= widthToMove) return;
      WheelController::rightRotate(1);
      WheelController::backward(backwardDistanceToSide);
      WheelController::leftRotate(1);
      WheelController::forward(forwardDistanceToFixPosition);
      if(leftWidthToMove <= widthToMove) return;
      leftWidthToMove -= widthToMove;
      // forward tp clean
      WheelController::backward(heightToMove);
      isPositionedUpper = true;
    }
    rightMoveToClean = true;
  }
  WheelController::stop();
}

void delayWithoutCpuStop(unsigned int ms, Timestamp &ts);

void delayWithoutCpuStop(unsigned int ms, Timestamp &ts){
  unsigned long start_mills = millis();
  while(millis() - start_mills < ms) yield();
  timestamp_add_milliseconds(ts, ms);
}

Timestamp machineInternalTimestamp;
constexpr unsigned long one_minute_mills = 60000;

void setup() {
  Serial.begin(115200);
  WheelController::setupPinMode();
  WheelController::stop();
  // receive KICData
  HTTPBroker::setup();
  String receiveString = HTTPBroker::receiveString();
  // String receiveString = "KIC:V3;31734;00500050;317351736;/";
  Serial.println(receiveString);
  kicData = KICCollection::convertToKIC(receiveString);
  if (kicData.board.height <= 0 && kicData.board.width <= 0) ESP.restart();
  // set machineInternalTimestamp with serverTimestamp
  machineInternalTimestamp.day = kicData.serverTimestamp.day;
  machineInternalTimestamp.hour_minute = kicData.serverTimestamp.hour_minute;
  timestamp_print(machineInternalTimestamp); // DEBUG:
  isOnceCleaned = false;
}

unsigned long mills_on_called;

void loop() {
  char current_day_index = machineInternalTimestamp.day - '0';
  if (current_day_index - '6' == 0) {
    // receive KICData
    String receiveString = HTTPBroker::receiveString();
    kicData = KICCollection::convertToKIC(receiveString);
    // set machineInternalTimestamp with serverTimestamp
    machineInternalTimestamp.day = kicData.serverTimestamp.day;
    machineInternalTimestamp.hour_minute = kicData.serverTimestamp.hour_minute;
  }
  if(millis() - mills_on_called >= one_minute_mills) {
    mills_on_called = millis();
    isOnceCleaned = false;
  }
  for (unsigned int len = 0; len < kicData.diagram.schedules[current_day_index].length; len++) {
    if (timestamp_compare_hour_minute(machineInternalTimestamp.hour_minute,
      kicData.diagram.schedules[current_day_index].hours[len]) && !isOnceCleaned) {
      isOnceCleaned = true;
      mills_on_called = millis();
      AutoClean(kicData.board);
      break;
    }
  }

  delayWithoutCpuStop(100, machineInternalTimestamp);
  timestamp_print(machineInternalTimestamp); // DEBUG:
}
