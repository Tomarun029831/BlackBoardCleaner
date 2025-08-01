#ifndef _COMMAND_EXECUTOR_HPP_
#define _COMMAND_EXECUTOR_HPP_

#include "../lib/WheelController.hpp"

// 最大カスタムコマンド数
#define MAX_CUSTOM_COMMANDS 10

struct CustomCommandEntry {
    char command;
    void (*handler)();
};

class CommandExecutor {
public:
    static bool executeCommand(char command);
    static bool executeCommand(char command, unsigned int parameter);
    static bool registerCustomCommand(char command, void (*handler)());
    
private:
    static CustomCommandEntry customCommands[MAX_CUSTOM_COMMANDS];
    static unsigned int numCustomCommands;
    
    static void executeCleaningSequence();
    static void executeEmergencyStop();
    static void executeStatusReport();
    static void (*findCustomHandler(char command))();
    
    // コマンドマッピング
    static const char CMD_FORWARD = 'W';
    static const char CMD_BACKWARD = 'S';
    static const char CMD_RIGHT = 'D';
    static const char CMD_LEFT = 'A';
    static const char CMD_STOP = 'Q';
    static const char CMD_AUTO_CLEAN = 'E';
    static const char CMD_STATUS = 'P';
    static const char CMD_EMERGENCY = '!';
};

#endif // _COMMAND_EXECUTOR_HPP_

