#include "../lib/HTTPBroker.hpp"
#include "../lib/WiFiConnector.hpp"
#include "../ENV.hpp"

namespace HTTPBroker {
    void setup() {
        while (!WiFiConnector::available()) {
            WiFiConnector::setup();
        }
    }

    bool available() {
        return WiFiConnector::available();
    }

    String receiveString() {

    }
}
