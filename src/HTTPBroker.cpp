#include "../lib/HTTPBroker.hpp"
#include "../config.hpp"
#include "../lib/WiFiConnector.hpp"
#include <Arduino.h>
#include <HTTPClient.h>
#include <WString.h>

namespace HTTPBroker {
static HTTPClient http;

void setup() { WiFiConnector::setup(); }

bool available() { return WiFiConnector::available(); }

String receiveString() {
  http.begin(CONFIG::APIENDPOINT);

  int httpCode = http.GET();
  while (httpCode <= 0)
    httpCode = http.GET();
  String payload = http.getString();

  http.end();
  return payload;
}
} // namespace HTTPBroker
