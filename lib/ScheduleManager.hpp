#ifndef _SCHEDULE_MANAGER_HPP_
#define _SCHEDULE_MANAGER_HPP_

#include "KICProtocolParser.hpp"

// 最大スケジュール数
#define MAX_SCHEDULES 20
// 各スケジュールの最大実行時刻数
#define MAX_EXECUTION_TIMES 10

struct ScheduleEntry {
    char command;
    unsigned int executionTimes[MAX_EXECUTION_TIMES]; // HHMM format (e.g., 0900 for 9:00 AM)
    unsigned int numExecutionTimes;
    bool isRepeating;
    unsigned long lastExecuted; // timestamp of last execution
};

class ScheduleManager {
public:
    ScheduleManager();

    // スケジュール管理
    bool addSchedule(const ParsedSchedule& schedule);
    bool removeSchedule(char command);
    void clearAllSchedules();

    // 実行チェック
    bool checkPendingCommands(char* pendingCommands, unsigned int maxCommands, unsigned int& numPending);
    void markCommandExecuted(char command);

    // 時刻管理
    void updateCurrentTime(unsigned long timestamp);
    unsigned long getCurrentTime() const;

    // デバッグ
    void printSchedules() const;

private:
    ScheduleEntry schedules[MAX_SCHEDULES];
    unsigned int numSchedules;
    unsigned long currentTime;

    bool isTimeToExecute(const ScheduleEntry& entry) const;
    unsigned int getCurrentHHMM() const;
    int findScheduleIndex(char command) const;
};

#endif // _SCHEDULE_MANAGER_HPP_

