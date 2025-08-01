#include "../test/CommandExecutorTestCase.hpp"
#include "../lib/CommandExecutor.hpp"
#include <Arduino.h>

// テスト用のカスタムコマンドハンドラー
void testCustomHandler1() {
    Serial.println("Custom handler 1 executed");
}

void testCustomHandler2() {
    Serial.println("Custom handler 2 executed");
}

void CommandExecutorTestCase::testBasicCommands() {
    Serial.println("--- testBasicCommands START ---");
    
    bool testPassed = true;
    
    // 基本コマンドのテスト
    Serial.println("Testing basic movement commands:");
    
    bool result1 = CommandExecutor::executeCommand('W', 1); // Forward
    if (!result1) {
        Serial.println("ERROR: Forward command failed");
        testPassed = false;
    }
    
    bool result2 = CommandExecutor::executeCommand('S', 1); // Backward
    if (!result2) {
        Serial.println("ERROR: Backward command failed");
        testPassed = false;
    }
    
    bool result3 = CommandExecutor::executeCommand('D'); // Right rotate
    if (!result3) {
        Serial.println("ERROR: Right rotate command failed");
        testPassed = false;
    }
    
    bool result4 = CommandExecutor::executeCommand('A'); // Left rotate
    if (!result4) {
        Serial.println("ERROR: Left rotate command failed");
        testPassed = false;
    }
    
    bool result5 = CommandExecutor::executeCommand('Q'); // Stop
    if (!result5) {
        Serial.println("ERROR: Stop command failed");
        testPassed = false;
    }
    
    bool result6 = CommandExecutor::executeCommand('P'); // Status
    if (!result6) {
        Serial.println("ERROR: Status command failed");
        testPassed = false;
    }
    
    Serial.print("testBasicCommands ");
    Serial.println(testPassed ? "PASSED" : "FAILED");
    Serial.println("--- testBasicCommands END ---");
}

void CommandExecutorTestCase::testCustomCommands() {
    Serial.println("--- testCustomCommands START ---");
    
    bool testPassed = true;
    
    // カスタムコマンドを登録
    bool reg1 = CommandExecutor::registerCustomCommand('X', testCustomHandler1);
    if (!reg1) {
        Serial.println("ERROR: Failed to register custom command X");
        testPassed = false;
    }
    
    bool reg2 = CommandExecutor::registerCustomCommand('Y', testCustomHandler2);
    if (!reg2) {
        Serial.println("ERROR: Failed to register custom command Y");
        testPassed = false;
    }
    
    // カスタムコマンドを実行
    Serial.println("Testing custom commands:");
    
    bool exec1 = CommandExecutor::executeCommand('X');
    if (!exec1) {
        Serial.println("ERROR: Custom command X failed");
        testPassed = false;
    }
    
    bool exec2 = CommandExecutor::executeCommand('Y');
    if (!exec2) {
        Serial.println("ERROR: Custom command Y failed");
        testPassed = false;
    }
    
    // 同じコマンドを再登録（更新）
    bool reg3 = CommandExecutor::registerCustomCommand('X', testCustomHandler2);
    if (!reg3) {
        Serial.println("ERROR: Failed to update custom command X");
        testPassed = false;
    }
    
    Serial.println("Testing updated custom command:");
    bool exec3 = CommandExecutor::executeCommand('X');
    if (!exec3) {
        Serial.println("ERROR: Updated custom command X failed");
        testPassed = false;
    }
    
    Serial.print("testCustomCommands ");
    Serial.println(testPassed ? "PASSED" : "FAILED");
    Serial.println("--- testCustomCommands END ---");
}

void CommandExecutorTestCase::testInvalidCommands() {
    Serial.println("--- testInvalidCommands START ---");
    
    bool testPassed = true;
    
    // 無効なコマンドのテスト
    Serial.println("Testing invalid commands:");
    
    bool result1 = CommandExecutor::executeCommand('Z'); // 未定義コマンド
    if (result1) {
        Serial.println("ERROR: Invalid command Z should have failed but succeeded");
        testPassed = false;
    }
    
    bool result2 = CommandExecutor::executeCommand('@'); // 特殊文字
    if (result2) {
        Serial.println("ERROR: Invalid command @ should have failed but succeeded");
        testPassed = false;
    }
    
    Serial.print("testInvalidCommands ");
    Serial.println(testPassed ? "PASSED" : "FAILED");
    Serial.println("--- testInvalidCommands END ---");
}

void CommandExecutorTestCase::runAllTests() {
    Serial.println("=== CommandExecutorTest START ===");
    testBasicCommands();
    testCustomCommands();
    testInvalidCommands();
    Serial.println("=== CommandExecutorTest END ===");
    Serial.flush();
}

