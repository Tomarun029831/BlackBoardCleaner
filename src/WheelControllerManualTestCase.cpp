#include "../lib/WheelController.hpp"
#include "../test/WheelControllerManualTestCase.hpp"
#include <Arduino.h>

namespace WheelControllerManualTestCase {

    static void delayAndStop(unsigned long ms) {
        delay(ms);
        WheelController::stop();
        delay(500);
    }

    void runAllTests() {
        Serial.println("=== WheelController Manual Test Start ===");

        WheelController::setupPinMode();
        WheelController::stop();

        Serial.println("[Test] Forward 10cm");
        WheelController::forward(10);
        delayAndStop(1500);

        Serial.println("[Test] Backward 10cm");
        WheelController::backward(10);
        delayAndStop(1500);

        Serial.println("[Test] Right Rotate");
        WheelController::rightRotate();
        delayAndStop(1000);

        Serial.println("[Test] Left Rotate");
        WheelController::leftRotate();
        delayAndStop(1000);

        Serial.println("[Test] Stop (all HIGH)");
        WheelController::stop();
        delay(1000);

        Serial.print("Pin state: ");
        Serial.println(WheelController::getAllPin());

        Serial.println("=== WheelController Manual Test End ===");
    }
}
