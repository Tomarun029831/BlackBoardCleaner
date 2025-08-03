#include "../lib/WiFiConnector.hpp"
#include "../ENV.hpp"
#include <WiFi.h>

namespace WiFiConnector {
    const unsigned long RETRY_INTERVAL_MS = 10000;

    void setup() {
        unsigned long startAttemptTime = millis();
        unsigned long lastRetryTime = 0;

        WiFi.begin(CONFIG::SSID, CONFIG::PASSWORD);

        while (WiFi.status() != WL_CONNECTED) {
            unsigned long currentMillis = millis();

            if (currentMillis - lastRetryTime >= RETRY_INTERVAL_MS) {
                WiFi.disconnect();
                WiFi.begin(CONFIG::SSID, CONFIG::PASSWORD);
                lastRetryTime = currentMillis;
            }

            delay(500);
        }
    }

    bool available() {
        return WiFi.status() == WL_CONNECTED;
    }
}
