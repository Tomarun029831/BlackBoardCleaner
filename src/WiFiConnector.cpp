#include "../lib/WiFiConnector.hpp"
#include "../config.hpp"
#include <HardwareSerial.h>
#include <WiFi.h>

namespace WiFiConnector {
// 1000 ms = 1 s
constexpr unsigned long RETRY_INTERVAL_MS = 25000;

void scanAPs() {
  Serial.println("Scanning for WiFi networks...");

  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("No networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found:");
    for (int i = 0; i < n; i++) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i)); // SSID
      Serial.print(" (RSSI: ");
      Serial.print(WiFi.RSSI(i)); // 電波強度
      Serial.print(") ");
      Serial.print((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open"
                                                              : "Encrypted");
      Serial.println();
    }
  }
  Serial.println("-----------------------");
}

void setup() {
  scanAPs();
  unsigned long lastRetryTime = 0;
  WiFi.mode(WIFI_STA);
  Serial.println(CONFIG::SSID);
  Serial.println(CONFIG::PASSWORD);
  WiFi.begin(CONFIG::SSID, CONFIG::PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("WiFi status: ");
    Serial.println(WiFi.status());
    unsigned long currentMillis = millis();

    if (currentMillis - lastRetryTime >= RETRY_INTERVAL_MS) {
      Serial.println("-- retry to connect --");
      WiFi.disconnect();
      WiFi.begin(CONFIG::SSID, CONFIG::PASSWORD);
      lastRetryTime = currentMillis;
    }
    delay(500);
  }
}

bool available() { return WiFi.status() == WL_CONNECTED; }
} // namespace WiFiConnector
