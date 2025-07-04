#include <WString.h>
#include <HardwareSerial.h>
#include "../lib/ScheduleGataway.h"

// WirelessGateway::WirelessGateway() {}

int WirelessGateway::available() {
    return true;
}

String WirelessGateway::receiveString() {
    String result = "";
    return result;
}

void WirelessGateway::sendString(String str) {
    // 何もしない（未実装）
}
