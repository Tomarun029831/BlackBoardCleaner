#include <WString.h>
#include <HardwareSerial.h>
#include "../lib/ScheduleGataway.h"

// SerialGateway::SerialGateway() {}

int SerialGateway::available() {
    return Serial.available();
}

String SerialGateway::receiveString() {
    String result = "";
    while (true) {
        if (Serial.available() <= 0) continue;
        char c = Serial.read();
        if (c == 'K') {  // 'K' から開始と仮定
            result += c;
            break;
        }
    }
    while (true) {
        if (Serial.available() <= 0) continue;
        char c = Serial.read();
        result += c;
        if (c == '/') break;  // 終端文字
    }
    return result;
}

void SerialGateway::sendString(String str) {
    Serial.print(str);
}
