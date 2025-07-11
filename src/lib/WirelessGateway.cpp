#include "../../update.hpp"
#include "../../ENV.hpp"
#include "../../lib/ScheduleGataway.hpp"
#include <HTTPClient.h>
#include <HardwareSerial.h>
#include <WString.h>
#include <WiFi.h>

int WirelessGateway::setup() {
  Serial.printf("Connecting to WiFi SSID: %s\n", CONFIG::SSID);
  WiFi.begin(CONFIG::SSID, CONFIG::PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    update();
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  return 0;
}

int WirelessGateway::available() {
  return WiFi.status() == WL_CONNECTED;
}

String WirelessGateway::receiveString() {
  if (!available()) {
    Serial.println("WiFi not connected");
    return "";
  }

  HTTPClient http;
  http.begin(CONFIG::APIENDPOINT);  // 例: http://example.com/api/getdata
  int httpCode = http.GET();

  String payload = "";
  if (httpCode > 0) {
    Serial.printf("GET Response code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      payload = http.getString();
    }
  } else {
    Serial.printf("GET failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  return payload;
}

void WirelessGateway::sendString(String str) {
  if (!available()) {
    Serial.println("WiFi not connected");
    return;
  }

  HTTPClient http;
  http.begin(CONFIG::APIENDPOINT);
    http.addHeader("Content-Type", "text/plain");
    int httpCode = http.POST(str);

  if (httpCode > 0) {
    Serial.printf("POST Response code: %d\n", httpCode);
  } else {
    Serial.printf("POST failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}
