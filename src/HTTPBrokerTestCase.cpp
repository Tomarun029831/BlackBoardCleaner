#include "../test/HTTPBrokerTestCase.hpp"
#include "../lib/HTTPBroker.hpp"
#include <HardwareSerial.h>
#include <WString.h>

namespace HTTPBrokerTestCase {
static bool assertKICHeader(String str1, String str2) {
  String kicheader1 = str1.substring(0, 3);
  String kicheader2 = str2.substring(0, 3);
  return kicheader1 == kicheader2;
}

bool testSetup() {
  HTTPBroker::setup();
  return HTTPBroker::available();
}

bool testAvailable() { return HTTPBroker::available(); }

bool testReceiveString() {
  String expected_header = "KIC";
  String received_string = HTTPBroker::receiveString();
  Serial.print("   >> HTTPBroker::receiveString: ");
  Serial.println(received_string);

  return assertKICHeader(expected_header, received_string);
}

void runAllTests() {
  Serial.println("HTTPClientTestCase");
  Serial.printf(" - testSetup %s\n", testSetup() ? "passed" : "failed");
  Serial.printf(" - testAvailable %s\n", testAvailable() ? "passed" : "failed");
  Serial.printf(" - testReceiveString %s\n",
                testReceiveString() ? "passed" : "failed");
}
} // namespace HTTPBrokerTestCase
