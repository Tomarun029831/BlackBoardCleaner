#include "../lib/KICProtocolParser.hpp"
#include <HardwareSerial.h>
#include <stdlib.h>
#include <ctype.h>

#define KICVERSION "KIC:V2"
#define KICSEGMENTCHAR ';'
#define KICEND "/"

KICInfo* KICProtocolParser::parse(const String& kicString) {
    if (!isValidKICFormat(kicString)) {
        Serial.println("Invalid KIC format");
        return nullptr;
    }

    KICInfo* kicInfo = (KICInfo*)malloc(sizeof(KICInfo));
    if (!kicInfo) {
        Serial.println("Memory allocation failed for KICInfo");
        return nullptr;
    }

    // 初期化
    kicInfo->timestamp = 0;
    kicInfo->boardHeight = 0;
    kicInfo->boardWidth = 0;
    kicInfo->numScheduleBlocks = 0;

    // KICENDをチェック
    if (!kicString.endsWith(KICEND)) {
        Serial.println("KIC string does not end with proper terminator");
        free(kicInfo);
        return nullptr;
    }

    int segmentStart = 0;
    int segmentIndex = 0;

    while (segmentStart < kicString.length() && segmentIndex < 20) { // 無限ループ防止
        int segmentEnd = kicString.indexOf(KICSEGMENTCHAR, segmentStart);
        if (segmentEnd == -1) {
            // 最後のセグメント（終端文字まで）
            segmentEnd = kicString.length() - 1; // '/'を除く
        }

        String segment = kicString.substring(segmentStart, segmentEnd);

        switch (segmentIndex) {
            case 0: // Version
                if (!validateVersion(segment)) {
                    Serial.println("Invalid version: " + segment);
                    free(kicInfo);
                    return nullptr;
                }
                kicInfo->version = segment;
                break;

            case 1: // Timestamp
                if (!validateTimestamp(segment)) {
                    Serial.println("Invalid timestamp: " + segment);
                    free(kicInfo);
                    return nullptr;
                }
                kicInfo->timestamp = segment.toInt();
                break;

            case 2: // Board dimensions
                if (!validateBoardDimensions(segment)) {
                    Serial.println("Invalid board dimensions: " + segment);
                    free(kicInfo);
                    return nullptr;
                }
                // 例: "01140334" -> height=114, width=334
                kicInfo->boardHeight = segment.substring(0, 4).toInt();
                kicInfo->boardWidth = segment.substring(4, 8).toInt();
                break;

            default: // Schedule blocks
                if (segment.length() > 0 && segment != "/" && 
                    kicInfo->numScheduleBlocks < MAX_SCHEDULE_BLOCKS) {
                    kicInfo->scheduleBlocks[kicInfo->numScheduleBlocks] = segment;
                    kicInfo->numScheduleBlocks++;
                }
                break;
        }

        segmentStart = segmentEnd + 1;
        segmentIndex++;

        // 終端に達した場合は終了
        if (segmentEnd >= kicString.length() - 1) {
            break;
        }
    }

    Serial.printf("Parsed KIC: version=%s, timestamp=%lu, board=%dx%d, schedules=%d\n",
                  kicInfo->version.c_str(), kicInfo->timestamp, 
                  kicInfo->boardHeight, kicInfo->boardWidth, 
                  kicInfo->numScheduleBlocks);

    return kicInfo;
}

ParsedSchedule* KICProtocolParser::parseScheduleBlock(const String& scheduleBlock) {
    if (scheduleBlock.length() < 1) {
        return nullptr;
    }

    ParsedSchedule* schedule = (ParsedSchedule*)malloc(sizeof(ParsedSchedule));
    if (!schedule) {
        Serial.println("Memory allocation failed for ParsedSchedule");
        return nullptr;
    }

    // 初期化
    schedule->command = scheduleBlock.charAt(0);
    schedule->numHours = 0;

    // 時間データを4文字ずつ解析
    for (int i = 1; i + 4 <= scheduleBlock.length() && schedule->numHours < MAX_HOURS_PER_SCHEDULE; i += 4) {
        String timeStr = scheduleBlock.substring(i, i + 4);
        unsigned int hour = timeStr.toInt();
        schedule->hours[schedule->numHours] = hour;
        schedule->numHours++;
    }

    Serial.printf("Parsed schedule: command=%c, hours=%d\n", 
                  schedule->command, schedule->numHours);

    return schedule;
}

bool KICProtocolParser::isValidKICFormat(const String& kicString) {
    // 基本的なフォーマットチェック
    if (kicString.length() < 10) return false;
    if (!kicString.startsWith(KICVERSION)) return false;
    if (!kicString.endsWith(KICEND)) return false;

    // セミコロンの数をチェック（最低3個必要）
    int semicolonCount = 0;
    for (int i = 0; i < kicString.length(); i++) {
        if (kicString.charAt(i) == KICSEGMENTCHAR) {
            semicolonCount++;
        }
    }

    return semicolonCount >= 3;
}

bool KICProtocolParser::validateVersion(const String& version) {
    return version == KICVERSION;
}

bool KICProtocolParser::validateTimestamp(const String& timestamp) {
    // 数値のみかチェック
    for (int i = 0; i < timestamp.length(); i++) {
        if (!isdigit(timestamp.charAt(i))) {
            return false;
        }
    }
    return timestamp.length() == 5; // 5桁の数値
}

bool KICProtocolParser::validateBoardDimensions(const String& dimensions) {
    if (dimensions.length() != 8) return false;

    // 全て数字かチェック
    for (int i = 0; i < dimensions.length(); i++) {
        if (!isdigit(dimensions.charAt(i))) {
            return false;
        }
    }

    return true;
}

void KICProtocolParser::freeKICInfo(KICInfo* kicInfo) {
    if (kicInfo) {
        free(kicInfo);
    }
}

void KICProtocolParser::freeParsedSchedule(ParsedSchedule* parsedSchedule) {
    if (parsedSchedule) {
        free(parsedSchedule);
    }
}

