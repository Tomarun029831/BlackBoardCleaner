#include "../lib/ScheduleManager.hpp"
#include <HardwareSerial.h>
#include <time.h>

ScheduleManager::ScheduleManager() : numSchedules(0), currentTime(0) {
    Serial.println("ScheduleManager initialized");
}

bool ScheduleManager::addSchedule(const ParsedSchedule& schedule) {
    // 既存のスケジュールを検索
    int existingIndex = findScheduleIndex(schedule.command);
    
    if (existingIndex >= 0) {
        // 既存のスケジュールを更新
        ScheduleEntry& entry = schedules[existingIndex];
        entry.numExecutionTimes = 0;
        
        for (unsigned int i = 0; i < schedule.numHours && i < MAX_EXECUTION_TIMES; i++) {
            entry.executionTimes[entry.numExecutionTimes] = schedule.hours[i];
            entry.numExecutionTimes++;
        }
        
        entry.lastExecuted = 0; // リセット
        Serial.printf("Updated schedule for command '%c'\n", schedule.command);
        return true;
    }
    
    // 新しいスケジュールを追加
    if (numSchedules >= MAX_SCHEDULES) {
        Serial.println("Cannot add schedule: maximum number of schedules reached");
        return false;
    }
    
    ScheduleEntry& newEntry = schedules[numSchedules];
    newEntry.command = schedule.command;
    newEntry.numExecutionTimes = 0;
    newEntry.isRepeating = true; // デフォルトで繰り返し
    newEntry.lastExecuted = 0;
    
    for (unsigned int i = 0; i < schedule.numHours && i < MAX_EXECUTION_TIMES; i++) {
        newEntry.executionTimes[newEntry.numExecutionTimes] = schedule.hours[i];
        newEntry.numExecutionTimes++;
    }
    
    numSchedules++;
    Serial.printf("Added new schedule for command '%c' with %d execution times\n", 
                  schedule.command, newEntry.numExecutionTimes);
    return true;
}

bool ScheduleManager::removeSchedule(char command) {
    int index = findScheduleIndex(command);
    if (index < 0) {
        return false;
    }
    
    // 配列を詰める
    for (unsigned int i = index; i < numSchedules - 1; i++) {
        schedules[i] = schedules[i + 1];
    }
    
    numSchedules--;
    Serial.printf("Removed schedule for command '%c'\n", command);
    return true;
}

void ScheduleManager::clearAllSchedules() {
    numSchedules = 0;
    Serial.println("All schedules cleared");
}

bool ScheduleManager::checkPendingCommands(char* pendingCommands, unsigned int maxCommands, unsigned int& numPending) {
    numPending = 0;
    
    for (unsigned int i = 0; i < numSchedules && numPending < maxCommands; i++) {
        if (isTimeToExecute(schedules[i])) {
            pendingCommands[numPending] = schedules[i].command;
            numPending++;
        }
    }
    
    return numPending > 0;
}

void ScheduleManager::markCommandExecuted(char command) {
    int index = findScheduleIndex(command);
    if (index >= 0) {
        schedules[index].lastExecuted = currentTime;
        Serial.printf("Marked command '%c' as executed at %lu\n", command, currentTime);
    }
}

void ScheduleManager::updateCurrentTime(unsigned long timestamp) {
    currentTime = timestamp;
}

unsigned long ScheduleManager::getCurrentTime() const {
    return currentTime;
}

void ScheduleManager::printSchedules() const {
    Serial.println("=== Current Schedules ===");
    for (unsigned int i = 0; i < numSchedules; i++) {
        const ScheduleEntry& entry = schedules[i];
        Serial.printf("Command '%c': ", entry.command);
        for (unsigned int j = 0; j < entry.numExecutionTimes; j++) {
            Serial.printf("%04d", entry.executionTimes[j]);
            if (j < entry.numExecutionTimes - 1) {
                Serial.print(", ");
            }
        }
        Serial.printf(" (last executed: %lu)\n", entry.lastExecuted);
    }
    Serial.println("========================");
}

bool ScheduleManager::isTimeToExecute(const ScheduleEntry& entry) const {
    if (currentTime == 0) return false; // 時刻が設定されていない
    
    unsigned int currentHHMM = getCurrentHHMM();
    
    // 実行時刻のいずれかと一致するかチェック
    for (unsigned int i = 0; i < entry.numExecutionTimes; i++) {
        if (currentHHMM == entry.executionTimes[i]) {
            // 同じ分に複数回実行されないようにチェック
            unsigned long currentMinute = currentTime / 60;
            unsigned long lastExecutedMinute = entry.lastExecuted / 60;
            
            if (currentMinute != lastExecutedMinute) {
                return true;
            }
        }
    }
    
    return false;
}

unsigned int ScheduleManager::getCurrentHHMM() const {
    if (currentTime == 0) return 0;
    
    // UNIXタイムスタンプからHHMM形式に変換
    time_t rawTime = (time_t)currentTime;
    struct tm* timeInfo = localtime(&rawTime);
    
    return (timeInfo->tm_hour * 100) + timeInfo->tm_min;
}

int ScheduleManager::findScheduleIndex(char command) const {
    for (unsigned int i = 0; i < numSchedules; i++) {
        if (schedules[i].command == command) {
            return i;
        }
    }
    return -1;
}

