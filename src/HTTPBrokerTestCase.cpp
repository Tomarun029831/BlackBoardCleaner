#include "../test/HTTPBrokerTestCase.hpp"
#include <WString.h>
#include <HardwareSerial.h>

namespace HTTPBrokerTestCase {
    static bool assertString(String str1, String str2) {
        return str1 == str2;
    }

    bool testSetup() {
        HTTPBroker::setup();
        return HTTPBroker::available();
    }

    bool testAvailable() {
        return HTTPBroker::available();
    }

    bool testReceiveString() {
        String expected_string = "KIC:V1;01437;01140334;008001200;20700090011001300;/";
        String string = HTTPBroker::receiveString();

        return assertString(expected_string, string);
    }

    void runAllTests(){
        Serial.println("HTTPClientTestCase");
        Serial.printf(" - testAvailable(expected to failed) %s\n", !testAvailable() ? "passed" : "failed");
        Serial.printf(" - testSetup %s\n", testSetup() ? "passed" : "failed");
        Serial.printf(" - testAvailable %s\n", testAvailable() ? "passed" : "failed");
        Serial.printf(" - testReceiveString %s\n", testReceiveString() ? "passed" : "failed");
    }
}
