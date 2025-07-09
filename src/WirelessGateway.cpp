#include "../ENV.h"
#include "../lib/ScheduleGataway.h"
#include <HTTPClient.h>
#include <HardwareSerial.h>
#include <WString.h>
#include <WiFi.h>

int WirelessGateway::setup() {
  Serial.printf("Connecting to WiFi SSID: %s\n", CONFIG::SSID);
  WiFi.begin(CONFIG::SSID, CONFIG::PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (WiFi.status() != WL_CONNECTED) {
    return 1;
  }
  HTTPClient http;

  http.begin(CONFIG::APIENDPOINT);
  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.printf("HTTP Response code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Response:");
      Serial.println(payload);
    }
  } else {
    Serial.printf("HTTP GET failed, error: %s\n",
                  http.errorToString(httpCode).c_str());
  }
  http.end();
  return 0;
}

int WirelessGateway::available() { return WiFi.status() == WL_CONNECTED; }

String WirelessGateway::receiveString() {
  String result = "";
  return result;
}

void WirelessGateway::sendString(String str) {
  // 何もしない（未実装）
}
