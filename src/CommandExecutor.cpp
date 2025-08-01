#include "../lib/CommandExecutor.hpp"
#include <HardwareSerial.h>

// 静的メンバーの初期化
CustomCommandEntry CommandExecutor::customCommands[MAX_CUSTOM_COMMANDS];
unsigned int CommandExecutor::numCustomCommands = 0;

bool CommandExecutor::executeCommand(char command) {
    return executeCommand(command, 1); // デフォルトパラメータ
}

bool CommandExecutor::executeCommand(char command, unsigned int parameter) {
    Serial.printf("Executing command '%c' with parameter %u\n", command, parameter);
    
    switch (command) {
        case CMD_FORWARD:
            WheelController::forward(parameter);
            Serial.println("Executed: forward");
            return true;
            
        case CMD_BACKWARD:
            WheelController::backward(parameter);
            Serial.println("Executed: backward");
            return true;
            
        case CMD_RIGHT:
            WheelController::rightRotate();
            Serial.println("Executed: right rotate");
            return true;
            
        case CMD_LEFT:
            WheelController::leftRotate();
            Serial.println("Executed: left rotate");
            return true;
            
        case CMD_STOP:
            WheelController::stop();
            Serial.println("Executed: stop");
            return true;
            
        case CMD_AUTO_CLEAN:
            executeCleaningSequence();
            Serial.println("Executed: auto cleaning");
            return true;
            
        case CMD_STATUS:
            executeStatusReport();
            Serial.println("Executed: status report");
            return true;
            
        case CMD_EMERGENCY:
            executeEmergencyStop();
            Serial.println("Executed: emergency stop");
            return true;
            
        default:
            // カスタムハンドラーをチェック
            void (*handler)() = findCustomHandler(command);
            if (handler != nullptr) {
                handler();
                Serial.printf("Executed custom command '%c'\n", command);
                return true;
            }
            
            Serial.printf("Unknown command '%c'\n", command);
            return false;
    }
}

bool CommandExecutor::registerCustomCommand(char command, void (*handler)()) {
    if (numCustomCommands >= MAX_CUSTOM_COMMANDS) {
        Serial.println("Cannot register custom command: maximum number reached");
        return false;
    }
    
    // 既存のコマンドをチェック
    for (unsigned int i = 0; i < numCustomCommands; i++) {
        if (customCommands[i].command == command) {
            // 既存のハンドラーを更新
            customCommands[i].handler = handler;
            Serial.printf("Updated custom command '%c'\n", command);
            return true;
        }
    }
    
    // 新しいカスタムコマンドを追加
    customCommands[numCustomCommands].command = command;
    customCommands[numCustomCommands].handler = handler;
    numCustomCommands++;
    
    Serial.printf("Registered custom command '%c'\n", command);
    return true;
}

void CommandExecutor::executeCleaningSequence() {
    constexpr unsigned int BLACKBOARD_HEIGHT = 105; // cm
    constexpr unsigned int BLACKBOARD_WIDTH = 335;  // cm
    constexpr unsigned int BODY_HEIGHT = 20;        // cm
    
    unsigned int segments = BLACKBOARD_HEIGHT / (2 * BODY_HEIGHT);
    
    Serial.printf("Starting cleaning sequence: %u segments\n", segments);
    
    for (unsigned int i = 0; i < segments; ++i) {
        Serial.printf("Cleaning segment %u/%u\n", i + 1, segments);
        
        WheelController::forward(BLACKBOARD_WIDTH);
        
        WheelController::rightRotate();
        WheelController::forward(BODY_HEIGHT);
        WheelController::rightRotate();
        
        WheelController::forward(BLACKBOARD_WIDTH);
        
        WheelController::leftRotate();
        WheelController::forward(BODY_HEIGHT);
        WheelController::leftRotate();
    }
    
    WheelController::stop();
    Serial.println("Cleaning sequence completed");
}

void CommandExecutor::executeEmergencyStop() {
    WheelController::stop();
    Serial.println("EMERGENCY STOP ACTIVATED");
    
    // 追加の安全措置があればここに実装
    // 例: LEDの点滅、警告音の再生など
}

void CommandExecutor::executeStatusReport() {
    String pinStatus = WheelController::getAllPin();
    Serial.printf("Motor pin status: %s\n", pinStatus.c_str());
    
    // 追加のステータス情報があればここに実装
    // 例: バッテリー残量、センサー状態など
}

void (*CommandExecutor::findCustomHandler(char command))() {
    for (unsigned int i = 0; i < numCustomCommands; i++) {
        if (customCommands[i].command == command) {
            return customCommands[i].handler;
        }
    }
    return nullptr;
}

