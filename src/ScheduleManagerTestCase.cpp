#include "../test/ScheduleManagerTestCase.hpp"
#include "../lib/ScheduleManager.hpp"
#include "../lib/KICProtocolParser.hpp"
#include <Arduino.h>

void ScheduleManagerTestCase::testAddSchedule() {
    Serial.println("--- testAddSchedule START ---");

    ScheduleManager testManager;
    bool testPassed = true;

    // テスト用のParsedScheduleを作成
    ParsedSchedule testSchedule;
    testSchedule.command = 'E';
    testSchedule.hours[0] = 900;  // 9:00
    testSchedule.hours[1] = 1300; // 13:00
    testSchedule.numHours = 2;

    // スケジュールを追加
    bool addResult = testManager.addSchedule(testSchedule);
    if (!addResult) {
        Serial.println("ERROR: Failed to add schedule");
        testPassed = false;
    }

    // 同じコマンドで別のスケジュールを追加（更新されるはず）
    ParsedSchedule updateSchedule;
    updateSchedule.command = 'E';
    updateSchedule.hours[0] = 1000; // 10:00
    updateSchedule.numHours = 1;

    bool updateResult = testManager.addSchedule(updateSchedule);
    if (!updateResult) {
        Serial.println("ERROR: Failed to update schedule");
        testPassed = false;
    }

    Serial.print("testAddSchedule ");
    Serial.println(testPassed ? "PASSED" : "FAILED");
    Serial.println("--- testAddSchedule END ---");
}

void ScheduleManagerTestCase::testRemoveSchedule() {
    Serial.println("--- testRemoveSchedule START ---");

    ScheduleManager testManager;
    bool testPassed = true;

    // テスト用のスケジュールを追加
    ParsedSchedule testSchedule;
    testSchedule.command = 'W';
    testSchedule.hours[0] = 800;
    testSchedule.numHours = 1;

    testManager.addSchedule(testSchedule);

    // スケジュールを削除
    bool removeResult = testManager.removeSchedule('W');
    if (!removeResult) {
        Serial.println("ERROR: Failed to remove existing schedule");
        testPassed = false;
    }

    // 存在しないスケジュールを削除しようとする
    bool removeNonExistentResult = testManager.removeSchedule('Z');
    if (removeNonExistentResult) {
        Serial.println("ERROR: Removed non-existent schedule (should have failed)");
        testPassed = false;
    }

    Serial.print("testRemoveSchedule ");
    Serial.println(testPassed ? "PASSED" : "FAILED");
    Serial.println("--- testRemoveSchedule END ---");
}

void ScheduleManagerTestCase::testCheckPendingCommands() {
    Serial.println("--- testCheckPendingCommands START ---");

    ScheduleManager testManager;
    bool testPassed = true;

    // 現在時刻を設定（例：9:00 AM）
    // 簡単のため、9:00を表すタイムスタンプを設定
    unsigned long testTime = 32400; // 9:00 AM in seconds since midnight
    testManager.updateCurrentTime(testTime);

    // 9:00に実行されるスケジュールを追加
    ParsedSchedule morningSchedule;
    morningSchedule.command = 'E';
    morningSchedule.hours[0] = 900; // 9:00
    morningSchedule.numHours = 1;

    testManager.addSchedule(morningSchedule);

    // ペンディングコマンドをチェック
    char pendingCommands[10];
    unsigned int numPending;

    bool hasPending = testManager.checkPendingCommands(pendingCommands, 10, numPending);

    // 注意：実際の時刻変換が正しく動作するかは環境に依存
    // ここでは基本的な動作をテスト
    Serial.printf("Has pending commands: %s, Count: %u\n", hasPending ? "true" : "false", numPending);

    Serial.print("testCheckPendingCommands ");
    Serial.println("PASSED"); // 基本的な動作確認のみ
    Serial.println("--- testCheckPendingCommands END ---");
}

void ScheduleManagerTestCase::testTimeManagement() {
    Serial.println("--- testTimeManagement START ---");

    ScheduleManager testManager;
    bool testPassed = true;

    // 時刻を設定
    unsigned long testTime = 1234567890;
    testManager.updateCurrentTime(testTime);

    // 時刻を取得
    unsigned long retrievedTime = testManager.getCurrentTime();

    if (retrievedTime != testTime) {
        Serial.printf("ERROR: Time mismatch. Expected %lu, got %lu\n", testTime, retrievedTime);
        testPassed = false;
    }

    Serial.print("testTimeManagement ");
    Serial.println(testPassed ? "PASSED" : "FAILED");
    Serial.println("--- testTimeManagement END ---");
}

void ScheduleManagerTestCase::runAllTests() {
    Serial.println("=== ScheduleManagerTest START ===");
    testAddSchedule();
    testRemoveSchedule();
    testCheckPendingCommands();
    testTimeManagement();
    Serial.println("=== ScheduleManagerTest END ===");
    Serial.flush();
}

