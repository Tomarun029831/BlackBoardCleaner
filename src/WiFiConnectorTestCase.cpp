#include "../test/WiFiConnectorTestCase.hpp"
#include "../lib/WiFiConnector.hpp"
#include <HardwareSerial.h>

namespace WiFiConnectorTestCase {
    bool testConnect() {
        WiFiConnector::setup();
        return WiFiConnector::available();
    }

    bool testAvailable() {
        return WiFiConnector::available();
    }

    void runAllTests(){
        Serial.println("WiFiConnectorTestCase");
        Serial.printf(" - testAvailable(expected to failed) %s\n", !testAvailable() ? "passed" : "failed");
        Serial.printf(" - testConnect %s\n", testConnect() ? "passed" : "failed");
        Serial.printf(" - testAvailable %s\n", testAvailable() ? "passed" : "failed");
    }
}
