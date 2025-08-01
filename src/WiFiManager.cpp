#include "../lib/WiFiManager.hpp"
#include <HardwareSerial.h>

bool WiFiManager::connect() {
    Serial.printf("Connecting to WiFi SSID: %s\n", CONFIG::SSID);
    WiFi.begin(CONFIG::SSID, CONFIG::PASSWORD);

    int attempts = 0;
    const int maxAttempts = 30; // 30秒でタイムアウト

    while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
        delay(1000);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        return true;
    } else {
        Serial.println("\nWiFi connection failed!");
        return false;
    }
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getLocalIP() {
    if (isConnected()) {
        return WiFi.localIP().toString();
    }
    return "";
}

