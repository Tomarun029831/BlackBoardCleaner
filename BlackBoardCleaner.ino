// === my tests ===
#include "test/KICCollectionTestCase.hpp"
#include "test/WiFiConnectorTestCase.hpp"
#include "test/HTTPClientTestCase.hpp"
#include "test/DiagramCollectionTestCase.hpp"
#include "test/WheelControllerManualTestCase.hpp"

// === Arduino  ===
#include <HardwareSerial.h>

// === core loop ===

/* === TODO ===

KIC:
  KICCollectionTestCase - OK
  KICCollection
  DiagramCollection

Connection:
  HTTPClientTestCase - OK
  WiFiConnectorTestCase - OK
  HTTPClient
  WiFiConnector

Controller:
  WheelControllerTestCase - OK
  WheelController
  AutoClean(HEIGHT, WIDTH)

WCP:
  WCPCollection

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

  // === Test ===
  KICCollectionTestCase::runAllTests();
  DiagramCollectionTestCase::runAllTests();
  WiFiConnectorTestCase::runAllTests();
  HTTPClientTestCase::runAllTests();
  WheeelControllerManualTestCase::runAllTests();
}

void loop() {}

