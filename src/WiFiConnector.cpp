#include "../lib/WiFiConnector.hpp"
#include "../config.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <HardwareSerial.h>
#include <WiFi.h>

namespace WiFiConnector {
constexpr unsigned long RETRY_INTERVAL_MS = 8000;

void setup() {
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to: ");
  Serial.println(CONFIG::SSID);
  WiFi.begin(CONFIG::SSID, CONFIG::PASSWORD);

  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(pdMS_TO_TICKS(500));
    Serial.print(".");

    if (millis() - startAttemptTime >= RETRY_INTERVAL_MS) {
      Serial.println("\n[WiFi] Retry connection...");
      WiFi.disconnect();
      vTaskDelay(pdMS_TO_TICKS(100));
      WiFi.begin(CONFIG::SSID, CONFIG::PASSWORD);
      startAttemptTime = millis();
    }

    // Serial.println(WiFi.status());
  }

  Serial.println("\n[WiFi] Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

bool available() { return WiFi.status() == WL_CONNECTED; }
} // namespace WiFiConnector
