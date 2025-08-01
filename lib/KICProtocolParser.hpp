#ifndef _KIC_PROTOCOL_PARSER_HPP_
#define _KIC_PROTOCOL_PARSER_HPP_

#include <WString.h>

// 最大スケジュールブロック数
#define MAX_SCHEDULE_BLOCKS 10
// 各スケジュールブロック内の最大時間エントリ数
#define MAX_HOURS_PER_SCHEDULE 10

struct KICInfo {
    String version;
    unsigned long timestamp;
    unsigned int boardHeight;
    unsigned int boardWidth;
    String scheduleBlocks[MAX_SCHEDULE_BLOCKS];
    unsigned int numScheduleBlocks; // 実際に使用されているブロック数
};

struct ParsedSchedule {
    char command;
    unsigned int hours[MAX_HOURS_PER_SCHEDULE];
    unsigned int numHours; // 実際に使用されている時間エントリ数
};

class KICProtocolParser {
public:
    // KICInfo* の代わりに KICInfo 構造体へのポインタを返す
    static KICInfo* parse(const String& kicString);
    // ParsedSchedule* の代わりに ParsedSchedule 構造体へのポインタを返す
    static ParsedSchedule* parseScheduleBlock(const String& scheduleBlock);
    static bool isValidKICFormat(const String& kicString);

    // メモリ解放関数
    static void freeKICInfo(KICInfo* kicInfo);
    static void freeParsedSchedule(ParsedSchedule* parsedSchedule);

private:
    static bool validateVersion(const String& version);
    static bool validateTimestamp(const String& timestamp);
    static bool validateBoardDimensions(const String& dimensions);
};

#endif // _KIC_PROTOCOL_PARSER_HPP_

