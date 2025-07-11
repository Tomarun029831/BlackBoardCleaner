#include "../../test/KICProtocolTestCase.hpp"
#include "../../lib/KIC.hpp"
#include <Arduino.h>
#include <WString.h>

void KICProtocolTestCase::testKICParser() {
    // -- setup --
    const String inputStr = "KIC:V1;1437;1090010001130;208000900;8;9;/";
    const char expectedCmds[]   = {'1', '2', '8', '9', '/'};
    const int  expectedCounts[] = {3,    2,   0,   0,   0};
    const int  expectedHours[][3] = {
        {900, 1000, 1130},
        {800, 900, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    const int numSchedules = sizeof(expectedCmds) / sizeof(expectedCmds[0]);

    Serial.print("Received String: ");
    Serial.println(inputStr);

    bool testPassed = true;
    SPool* spool = KIC::convertToSPool(inputStr);

    // -- test --
    if (spool->count != numSchedules) {
        Serial.print("ERROR: schedule count mismatch. expected ");
        Serial.print(numSchedules);
        Serial.print(", actual ");
        Serial.println(spool->count);
        testPassed = false;
    }

    for (unsigned int i = 0; i < spool->count && i < numSchedules; i++) {
        Schedule* schedule = *(spool->schedules + i);

        if (schedule->cmd != expectedCmds[i]) {
            Serial.print("ERROR: cmd mismatch at schedule ");
            Serial.print(i);
            Serial.print(". expected '");
            Serial.print(expectedCmds[i]);
            Serial.print("', got '");
            Serial.print(schedule->cmd);
            Serial.println("'");
            testPassed = false;
        }

        if (schedule->count != expectedCounts[i]) {
            Serial.print("ERROR: hours count mismatch at schedule ");
            Serial.print(i);
            Serial.print(". expected ");
            Serial.print(expectedCounts[i]);
            Serial.print(", got ");
            Serial.println(schedule->count);
            testPassed = false;
        }

        for (unsigned int j = 0; j < schedule->count; j++) {
            if (schedule->hours[j] != expectedHours[i][j]) {
                Serial.print("ERROR: hour mismatch at schedule ");
                Serial.print(i);
                Serial.print(", index ");
                Serial.print(j);
                Serial.print(". expected ");
                Serial.print(expectedHours[i][j]);
                Serial.print(", got ");
                Serial.println(schedule->hours[j]);
                testPassed = false;
            }
        }
    }

    // -- result --
    Serial.print("testKICParser ");
    Serial.println(testPassed ? "passed" : "failed");

    // -- teardown --
    freeSPool(spool);
}

void KICProtocolTestCase::runAllTests() {
    Serial.println("=== KICProtocolTest START ===");
    testKICParser();
    Serial.println("=== KICProtocolTest END ===");
    Serial.flush();
}
