// === my tests ===
#include "test/KICCollectionTestCase.hpp"
#include "test/WiFiConnectorTestCase.hpp"
#include "test/HTTPBrokerTestCase.hpp"
#include "test/CleaningDiagramCollectionTestCase.hpp"
#include "test/WheelControllerManualTestCase.hpp"

// === Arduino  ===
#include <HardwareSerial.h>

// === my libs ===
#include "./lib/WheelController.hpp"

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
  WheelController - NEED MANUAL TEST
  AutoClean(HEIGHT, WIDTH)

*/

/*
BlackBoard:
HEIGHT = 335 cm
WIDTH = 105 cm

MACHINE:
HEIGHT = 20 cm
WIDTH = 15 cm
*/

void setup() {
  Serial.begin(115200);
  WheelController::stop();

  // // === Test ===
  // CleaningDiagramCollectionTestCase::runAllTests(); // passed
  // KICCollectionTestCase::runAllTests(); // passed
  //
  // WiFiConnectorTestCase::runAllTests(); // passed
  // HTTPBrokerTestCase::runAllTests(); // passed
  //
  // WheelControllerManualTestCase::runAllTests(); // passed
}

void loop() {}

