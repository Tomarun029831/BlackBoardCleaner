// === my tests ===
// #include "test/KICCollectionTestCase.hpp"
#include "test/WiFiConnectorTestCase.hpp"
#include "test/HTTPBrokerTestCase.hpp"
// #include "test/CleaningDiagramCollectionTestCase.hpp"
// #include "test/WheelControllerManualTestCase.hpp"

// CleaningDiagramCollectionTestCase::runAllTests(); // passed
// KICCollectionTestCase::runAllTests(); // passed

// WiFiConnectorTestCase::runAllTests(); // passed
// HTTPBrokerTestCase::runAllTests(); // passed

// WheelControllerManualTestCase::runAllTests(); // passed

// === Arduino  ===
#include <HardwareSerial.h>
#include <cmath>

// === my libs ===
#include "./lib/WheelController.hpp"
#include "./lib/KICCollection.hpp"
#include "./lib/HTTPBroker.hpp"
#include "./lib/Timestamp.hpp"

/* === TODO ===
KIC:
  KICCollectionTestCase - OK
  KICCollection - OK
  DiagramCollection - OK
Connection:
  HTTPClientTestCase - OK
  WiFiConnectorTestCase - OK
  HTTPClient - OK
  WiFiConnector - OK
Controller:
  WheelControllerTestCase - OK
  WheelController - OK
  AutoClean(HEIGHT, WIDTH)
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

static KICCollection::KICData kicData;

static constexpr unsigned int machineWidth = 22;   // cm
static constexpr unsigned int machineHeight = 23;  // cm
static bool rightMoveToClean = true;
static bool isPositionedUpper = true;

static void AutoClean(const KICCollection::Board boardSize) {
  const unsigned int heightToMove = boardSize.height - machineHeight;
  unsigned int leftWidthToMove = boardSize.width - machineWidth;
  constexpr unsigned int forwardDistanceToSide = 5;
  constexpr unsigned int backwardDistanceToSide = 5;
  constexpr unsigned int forwardDistanceToFixPosition = 5;
  constexpr unsigned int backwardDistanceToFixPosition = 5;

  if(rightMoveToClean && isPositionedUpper){
    while(true){
      // forward to clean
      WheelController::forward(heightToMove);
      isPositionedUpper = false;

      // slide to side at under position
      if(leftWidthToMove <= machineWidth) return;
      WheelController::rightRotate(1);
      WheelController::backward(backwardDistanceToSide);
      WheelController::leftRotate(1);
      WheelController::forward(forwardDistanceToFixPosition);
      leftWidthToMove -= machineWidth;
      // backward to clean
      WheelController::backward(heightToMove);
      isPositionedUpper = true;

      // slide to side at upper position
      if(leftWidthToMove <= machineWidth) return;
      WheelController::leftRotate(1);
      WheelController::forward(forwardDistanceToSide);
      WheelController::rightRotate(1);
      WheelController::backward(backwardDistanceToFixPosition);
      leftWidthToMove -= machineWidth;
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
      if(leftWidthToMove <= machineWidth) return;
      WheelController::rightRotate(1);
      WheelController::forward(forwardDistanceToSide);
      WheelController::leftRotate(1);
      WheelController::backward(backwardDistanceToFixPosition);
      leftWidthToMove -= machineWidth;
      // forward to clean
      WheelController::forward(heightToMove);
      isPositionedUpper = false;

      // slide to side at under position
      if(leftWidthToMove <= machineWidth) return;
      WheelController::rightRotate(1);
      WheelController::backward(backwardDistanceToSide);
      WheelController::leftRotate(1);
      WheelController::forward(forwardDistanceToFixPosition);
      leftWidthToMove -= machineWidth;
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
      if(leftWidthToMove <= machineWidth) return;
      WheelController::leftRotate(1);
      WheelController::backward(backwardDistanceToSide);
      WheelController::rightRotate(1);
      WheelController::forward(forwardDistanceToFixPosition);
      leftWidthToMove -= machineWidth;
      // backward to clean
      WheelController::backward(heightToMove);
      isPositionedUpper = true;

      // slide to side at upper position
      if(leftWidthToMove <= machineWidth) return;
      WheelController::rightRotate(1);
      WheelController::forward(forwardDistanceToSide);
      WheelController::leftRotate(1);
      WheelController::backward(backwardDistanceToFixPosition);
      leftWidthToMove -= machineWidth;
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
      if(leftWidthToMove <= machineWidth) return;
      WheelController::rightRotate(1);
      WheelController::forward(forwardDistanceToSide);
      WheelController::leftRotate(1);
      WheelController::backward(backwardDistanceToFixPosition);
      leftWidthToMove -= machineWidth;
      // backward to clean
      WheelController::forward(heightToMove);
      isPositionedUpper = false;

      // slide to side at under position
      if(leftWidthToMove <= machineWidth) return;
      WheelController::rightRotate(1);
      WheelController::backward(backwardDistanceToSide);
      WheelController::leftRotate(1);
      WheelController::forward(forwardDistanceToFixPosition);
      leftWidthToMove -= machineWidth;
      // forward tp clean
      WheelController::backward(heightToMove);
      isPositionedUpper = true;
    }
    rightMoveToClean = true;
  }
  WheelController::stop();
}

Timestamp machineInternalTimestamp;
static unsigned long last_mills;
unsigned long one_minute_mills = 60000; // 60000

void setup() {
  Serial.begin(115200); // Debug
  WheelController::stop();

  // receive KICData
  // HTTPBroker::setup();
  // String receiveString = HTTPBroker::receiveString();
  // Serial.println(receiveString);
  String receiveString = "KIC:V3;00000;01140334;008000821;10010090011001300;/";
  kicData = KICCollection::convertToKIC(receiveString);
  // set machineInternalTimestamp with serverTimestamp
  machineInternalTimestamp.day = kicData.serverTimestamp.day;
  machineInternalTimestamp.hour_minute = kicData.serverTimestamp.hour_minute;

  // WheelController::rightRotate(1);
  // delay(180);
  // WheelController::forward(14);
  // delay(180);

  // WheelController::rightRotate(1);
  // delay(180);
  // WheelController::forward(14);
  // delay(180);

  last_mills = millis();
  timestamp_add_minutes(machineInternalTimestamp, last_mills / one_minute_mills);
}

static unsigned long minute_counter = 0;

void loop() {
  unsigned long current_millis = millis();
  unsigned long elapsed_minutes = current_millis / one_minute_mills;

  WheelController::forward(10);
  WheelController::backward(10);

  if (elapsed_minutes > minute_counter) {
    unsigned int minutes_to_add = elapsed_minutes - minute_counter;
    timestamp_add_minutes(machineInternalTimestamp, minutes_to_add);
    minute_counter = elapsed_minutes;

    // Serial.print("Time updated (");
    // Serial.print(minutes_to_add);
    // Serial.print(" min) - HHMM: ");
    // Serial.println(machineInternalTimestamp.hour_minute);
    // Serial.print("Day: ");
    // Serial.println(machineInternalTimestamp.day);
  }

  char current_day_index = machineInternalTimestamp.day - '0';
  for (unsigned int len = 0; len < kicData.diagram.schedules[current_day_index].length; len++) {
    if (timestamp_compare_hour_minute(machineInternalTimestamp.hour_minute,
      kicData.diagram.schedules[current_day_index].hours[len])) {
      Serial.println("=== AutoClean triggered! ===");
      // AutoClean(kicData.board);
      break;  // 同じ時刻で複数回実行を防ぐ
    }
  }

  delay(100);
}
