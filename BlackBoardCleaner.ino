// === my tests ===
#include "test/KICCollectionTestCase.hpp"
#include "test/WiFiConnectorTestCase.hpp"
#include "test/HTTPBrokerTestCase.hpp"
#include "test/CleaningDiagramCollectionTestCase.hpp"
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
  HTTPClient - OK
  WiFiConnector - OK

Controller:
  WheelControllerTestCase - OK
  WheelController - NEED MANUAL TEST
  AutoClean(HEIGHT, WIDTH)

Memery:
  MemeryAllocetor

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
  CleaningDiagramCollectionTestCase::runAllTests();
  WiFiConnectorTestCase::runAllTests();
  HTTPBrokerTestCase::runAllTests();
  WheeelControllerManualTestCase::runAllTests();
}

void loop() {}

