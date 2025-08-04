#include "../test/KICCollectionTestCase.hpp"
#include "../lib/KICCollection.hpp"
#include "../lib/CleaningDiagramCollection.hpp"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <WString.h>

namespace KICCollectionTestCase {
    static CleaningDiagramCollection::DaySchedule* createDaySchedule(char header, unsigned int* hours, unsigned int length) {
        if (hours == nullptr && length > 0) {
            return nullptr;
        }

        CleaningDiagramCollection::DaySchedule* schedule = (CleaningDiagramCollection::DaySchedule*)calloc(1, sizeof(CleaningDiagramCollection::DaySchedule));
        if (schedule == nullptr) {
            return nullptr;
        }

        schedule->header = header;
        schedule->length = length;

        if (length > 0) {
            schedule->hours = (unsigned int*)calloc(length, sizeof(unsigned int));
            if (schedule->hours == nullptr) {
                free(schedule);
                return nullptr;
            }

            for (unsigned int i = 0; i < length; ++i) {
                schedule->hours[i] = hours[i];
            }
        } else {
            schedule->hours = nullptr;
        }

        return schedule;
    }

    static KICCollection::Board createBoard(unsigned int height, unsigned int width) {
        KICCollection::Board board;
        board.height = height;
        board.width = width;
        return board;
    }

    static CleaningDiagramCollection::CleaningDiagram createCleaningDiagram(CleaningDiagramCollection::DaySchedule** schedules, unsigned int scheduleCount) {
        CleaningDiagramCollection::CleaningDiagram diagram;
        diagram.length = scheduleCount;

        if (scheduleCount == 0 || schedules == nullptr) {
            diagram.schedules = nullptr;
            return diagram;
        }

        diagram.schedules = (CleaningDiagramCollection::DaySchedule*)calloc(scheduleCount, sizeof(CleaningDiagramCollection::DaySchedule));
        if (diagram.schedules == nullptr) {
            diagram.length = 0;
            return diagram;
        }

        for (unsigned int i = 0; i < scheduleCount; ++i) {
            if (schedules[i] != nullptr) {
                diagram.schedules[i] = *schedules[i];
                // 時間配列を個別にコピー
                if (schedules[i]->length > 0) {
                    diagram.schedules[i].hours = (unsigned int*)calloc(schedules[i]->length, sizeof(unsigned int));
                    if (diagram.schedules[i].hours != nullptr) {
                        for (unsigned int j = 0; j < schedules[i]->length; ++j) {
                            diagram.schedules[i].hours[j] = schedules[i]->hours[j];
                        }
                    }
                }
            }
        }

        return diagram;
    }

    static KICCollection::KICData* createKICData(KICCollection::ServerTimestamp* serverTimestamp, KICCollection::Board board, CleaningDiagramCollection::CleaningDiagram diagram) {
        KICCollection::KICData* kicData = (KICCollection::KICData*)calloc(1, sizeof(KICCollection::KICData));
        if (kicData == nullptr) {
            return nullptr;
        }

        kicData->serverTimestamp = *serverTimestamp;
        kicData->board = board;
        kicData->diagram = diagram;

        return kicData;
    }

    static bool assertString(const char *str1, const char *str2) {
        if (str1 == nullptr || str2 == nullptr) return false;
        return strcmp(str1, str2) == 0;
    }

    static bool assertDaySchedule(const CleaningDiagramCollection::DaySchedule* a, const CleaningDiagramCollection::DaySchedule* b) {
        if (a == nullptr || b == nullptr) return false;
        if (a->header != b->header) return false;
        if (a->length != b->length) return false;

        for (unsigned int i = 0; i < a->length; ++i) {
            if (a->hours[i] != b->hours[i]) return false;
        }
        return true;
    }

    static bool assertServerTimestamp(const KICCollection::ServerTimestamp *timestamp1, const KICCollection::ServerTimestamp *timestamp2) {
        if (timestamp1 == nullptr || timestamp2 == nullptr) return false;

        // Compare day field
        if (timestamp1->day != timestamp2->day) return false;

        // Compare hour_minute field
        if (timestamp1->hour_minute != timestamp2->hour_minute) return false;

        return true;
    }

    static bool assertKICData(const KICCollection::KICData *data1, const KICCollection::KICData *data2) {
        if (data1 == nullptr || data2 == nullptr) return false;

        // check isNull flag
        if (data1->isNull != data2->isNull) return false;

        // check serverTimestamp (instead of serverSendTime)
        if (!assertServerTimestamp(&(data1->serverTimestamp), &(data2->serverTimestamp))) return false;

        // check Board
        if (data1->board.height != data2->board.height) return false;
        if (data1->board.width  != data2->board.width) return false;

        // check CleaningDiagram
        if (data1->diagram.length != data2->diagram.length) return false;
        for (unsigned int i = 0; i < data1->diagram.length; ++i) {
            if (!assertDaySchedule(&data1->diagram.schedules[i], &data2->diagram.schedules[i])) {
                return false;
            }
        }
        return true;
    }

    bool testKICLexer(){
        String testString = "KIC:V2;01437;01140334;008001200;20700090011001300;/";
        const char *expected_kicHeader = KICCollection::KICVERSION;
        const char *expected_serverSendTime = "01437";
        const char *expected_boardSize = "01140334";
        const char *expected_cleanDiagram = "008001200;20700090011001300;/";

        char kicHeader[KICCollection::KICHEADERLENGTH];
        char serverSendTime[KICCollection::SERVERSENDTIMELENGTH];
        char boardSize[KICCollection::BOARDSIZELENGTH];
        char *cleanDiagram = nullptr;

        KICCollection::KICLexer(testString, kicHeader, serverSendTime, boardSize, &cleanDiagram);

        bool isPassed = true;
        if(!assertString(kicHeader, expected_kicHeader)) isPassed = false;
        if(!assertString(serverSendTime, expected_serverSendTime)) isPassed = false;
        if(!assertString(boardSize, expected_boardSize)) isPassed = false;
        if(!assertString(cleanDiagram, expected_cleanDiagram)) isPassed = false;

        free(cleanDiagram);

        return isPassed;
    }

    bool testKICParser() {
        const char *test_kicHeader = KICCollection::KICVERSION;
        const char *test_serverSendTime = "01437";
        const char *test_boardSize = "01140334";
        const char *test_cleanDiagram = "008001200;20700090011001300;/";

        unsigned int serverSendHour[] = {1437};
        KICCollection::ServerTimestamp* expectedServerTimestamp = (KICCollection::ServerTimestamp *)calloc(1, sizeof(KICCollection::ServerTimestamp));
        expectedServerTimestamp->day = '0';
        expectedServerTimestamp->hour_minute = 1437;

        KICCollection::Board expectedBoard = createBoard(114, 334);

        unsigned int schedule0Hours[] = {800, 1200};
        unsigned int schedule1Hours[] = {700, 900, 110, 1300};
        CleaningDiagramCollection::DaySchedule* schedule0 = createDaySchedule('0', schedule0Hours, 2);
        CleaningDiagramCollection::DaySchedule* schedule1 = createDaySchedule('2', schedule1Hours, 4);

        CleaningDiagramCollection::DaySchedule* schedules[] = {schedule0, schedule1};
        CleaningDiagramCollection::CleaningDiagram expectedDiagram = createCleaningDiagram(schedules, 2);

        KICCollection::KICData* expected_kicData = createKICData(expectedServerTimestamp, expectedBoard, expectedDiagram);

        KICCollection::KICData kicData = KICCollection::KICParser(test_kicHeader, test_serverSendTime, test_boardSize, test_cleanDiagram);
        bool isPassed = kicData.isNull ? false : assertKICData(&kicData, expected_kicData);

        free(expectedServerTimestamp);
        free(schedule0);
        free(schedule1);
        free(expected_kicData);

        return isPassed;
    }

    void runAllTests(){
        Serial.println("KICCollectionTestCase");
        Serial.printf(" - testKICLexer %s\n", testKICLexer() ? "passed" : "failed");
        Serial.printf(" - testKICParser %s\n", testKICParser() ? "passed" : "failed");
    }
};
