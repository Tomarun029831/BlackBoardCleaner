// === Arduino  ===
#include <HardwareSerial.h>
#include <cmath>

// === my libs ===
#include "./lib/WheelController.hpp"
#include "./lib/KICCollection.hpp"
#include "./lib/HTTPBroker.hpp"
#include "./lib/Timestamp.hpp"

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

static KICCollection::KICData kicData;

static constexpr unsigned int machineWidth = 22;   // cm
static constexpr unsigned int machineHeight = 23;  // cm
static bool rightMoveToClean = true;
static bool isPositionedUpper = true;

static void AutoClean(const KICCollection::Board boardSize) {
  const unsigned int heightToMove = boardSize.height - machineHeight;
  unsigned int leftWidthToMove = boardSize.width - machineWidth;
  constexpr unsigned int widthToMove = 11; // 11cm
  constexpr unsigned int forwardDistanceToSide = 40;
  constexpr unsigned int backwardDistanceToSide = 40;
  constexpr unsigned int forwardDistanceToFixPosition = 40;
  constexpr unsigned int backwardDistanceToFixPosition = 60; // 40

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
      leftWidthToMove -= widthToMove;
      // forward tp clean
      WheelController::backward(heightToMove);
      isPositionedUpper = true;
    }
    rightMoveToClean = true;
  }
  WheelController::stop();
}

void setup() {
  Serial.begin(115200);
  WheelController::stop();

  KICCollection::Board test_board;
  test_board.height = 60;
  test_board.width = 75;
  AutoClean(test_board);
}

void loop() {}
