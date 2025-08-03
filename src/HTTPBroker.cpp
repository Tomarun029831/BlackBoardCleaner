#include "../lib/HTTPBroker.hpp"
#include "../lib/WiFiConnector.hpp"
#include "../ENV.hpp"
#include <Arduino.h>
#include <WString.h>
#include <HTTPClient.h>

namespace HTTPBroker {
    static HTTPClient http;

    void setup() {
        WiFiConnector::setup();
    }

    bool available() {
        return WiFiConnector::available();
    }

    String receiveString() {
        http.begin(CONFIG::APIENDPOINT);

        int httpCode = http.GET();
        if (httpCode <= 0) return "";
        String payload = http.getString();

        http.end();
        return payload;
    }
}
