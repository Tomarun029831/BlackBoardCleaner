#include "../../test/WiFiConnectionTestCase.hpp"
#include "../../ENV.hpp"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <WiFi.h>

void WiFiConnectionTestCase::connect() {
    Serial.println("[WiFi] Connecting...");
    WiFi.begin(CONFIG::SSID, CONFIG::PASSWORD);

    unsigned long startAttemptTime = millis();
    const unsigned long timeout = INTMAX_MAX;

    while (WiFi.status() != WL_CONNECTED) {
        delay(5000);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n[WiFi] Connected!");
        Serial.print("[WiFi] IP address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\n[WiFi] Connection failed.");
    }
}

void WiFiConnectionTestCase::runAllTests() {
    Serial.println("=== WiFiConnectionTest START ===");
    connect();
    Serial.println("=== WiFiConnectionTest END ===\n");
    Serial.flush();
}
