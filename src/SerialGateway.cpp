#include <WString.h>
#include <HardwareSerial.h>
#include "../lib/ScheduleGateway.hpp"

static constexpr uint baudRate = 115200;

bool SerialGateway::setup() {
    Serial.begin(baudRate); 
    while (!Serial); // HACK: wait for opening Serial Port infinitely

    return Serial.available() > 0 ? true : false;
}

static bool isExisting(){
    bool isSerialSetuped = Serial ? true : false;
    return isSerialSetuped;
}

bool SerialGateway::available() {
    bool isSerialSetuped = isExisting() ? (Serial.available() > 0 ? true : false) : false;
    return isSerialSetuped;
}

String SerialGateway::receiveString() {
    String result = "";
    if(!SerialGateway::available()) return result;

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
    if(!isExisting()) return;

    Serial.print(str);
}
