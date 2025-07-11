#include "../../test/HTTPResponseTestCase.hpp"
#include "../../ENV.hpp"
#include <HTTPClient.h>
#include <Arduino.h>
#include <WiFi.h>
#include <HardwareSerial.h>

void HTTPResponseTestCase::runAllTests()
{
    Serial.println("=== HTTPResponseTest START ===");

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[HTTP] WiFi not connected. Test aborted.");
        return;
    }

    HTTPClient http;
    http.begin(CONFIG::APIENDPOINT);

    int httpCode = http.GET();
    if (httpCode > 0) {
        Serial.printf("[HTTP] Response code: %d\n", httpCode);
        String payload = http.getString();
        Serial.println("[HTTP] Payload:");
        Serial.println(payload);
    } else {
        Serial.printf("[HTTP] Request failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    Serial.println("=== HTTPResponseTest END ===\n");
    Serial.flush();
}
