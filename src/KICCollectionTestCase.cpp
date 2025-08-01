#include "../test/KICCollectionTestCase.hpp"
#include "../lib/KICCollection.hpp"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <WString.h>

namespace KICCollectionTestCase {
    static DaySchedule* createDaySchedule(char header, unsigned int* hours, unsigned int length) {
        if (hours == nullptr && length > 0) {
            return nullptr;
        }

        DaySchedule* schedule = (DaySchedule*)calloc(1, sizeof(DaySchedule));
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

    static Board createBoard(unsigned int height, unsigned int width) {
        Board board;
        board.height = height;
        board.width = width;
        return board;
    }

    static CleaningDiagram createCleaningDiagram(DaySchedule** schedules, unsigned int scheduleCount) {
        CleaningDiagram diagram;
        diagram.length = scheduleCount;

        if (scheduleCount == 0 || schedules == nullptr) {
            diagram.schedules = nullptr;
            return diagram;
        }

        diagram.schedules = (DaySchedule*)calloc(scheduleCount, sizeof(DaySchedule));
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

    static KICData* createKICData(DaySchedule* serverSendTime, Board board, CleaningDiagram diagram) {
        KICData* kicData = (KICData*)calloc(1, sizeof(KICData));
        if (kicData == nullptr) {
            return nullptr;
        }

        kicData->serverSendTime = serverSendTime;
        kicData->board = board;
        kicData->diagram = diagram;

        return kicData;
    }

    static bool assertString(const char *str1, const char *str2) {
        if (str1 == nullptr || str2 == nullptr) return false;
        return strcmp(str1, str2) == 0;
    }

    static bool assertDaySchedule(const DaySchedule* a, const DaySchedule* b) {
        if (a == nullptr || b == nullptr) return false;
        if (a->header != b->header) return false;
        if (a->length != b->length) return false;

        for (unsigned int i = 0; i < a->length; ++i) {
            if (a->hours[i] != b->hours[i]) return false;
        }
        return true;
    }

    static bool assertKICData(const KICData *data1, const KICData *data2) {
        if (data1 == nullptr || data2 == nullptr) return false;

        // check serverSendTime (DaySchedule*)
        if (!assertDaySchedule(data1->serverSendTime, data2->serverSendTime)) return false;

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
        const char *expected_kicHeader = KICVERSION;
        const char *expected_serverSendTime = "01437";
        const char *expected_boardSize = "01140334";
        const char *expected_cleanDiagram = "008001200;20700090011001300;";

        char *kicHeader = "";
        char *serverSendTime = "";
        char *boardSize = "";
        char *cleanDiagram = "";

        KICCollection::KICLexer(testString, kicHeader, serverSendTime, boardSize, cleanDiagram);

        bool isPassed = true;
        if(!assertString(kicHeader, expected_kicHeader)) isPassed = false;
        if(!assertString(serverSendTime, expected_serverSendTime)) isPassed = false;
        if(!assertString(boardSize, expected_boardSize)) isPassed = false;
        if(!assertString(cleanDiagram, expected_cleanDiagram)) isPassed = false;

        free(kicHeader);
        free(serverSendTime);
        free(boardSize);
        free(cleanDiagram);

        return isPassed;
    }

    bool testKICParser() {
        const char *test_kicHeader = KICVERSION;
        const char *test_serverSendTime = "01437";
        const char *test_boardSize = "01140334";
        const char *test_cleanDiagram = "008001200;20700090011001300;";

        unsigned int serverSendHour[] = {1437};
        DaySchedule* expectedServerSendTime = createDaySchedule('0', serverSendHour, 1);

        Board expectedBoard = createBoard(114, 334);

        unsigned int schedule0Hours[] = {800, 1200};
        unsigned int schedule1Hours[] = {700, 900, 110, 1300};
        DaySchedule* schedule0 = createDaySchedule('0', schedule0Hours, 2);
        DaySchedule* schedule1 = createDaySchedule('2', schedule1Hours, 4);

        DaySchedule* schedules[] = {schedule0, schedule1};
        CleaningDiagram expectedDiagram = createCleaningDiagram(schedules, 2);

        KICData* expected_kicData = createKICData(expectedServerSendTime, expectedBoard, expectedDiagram);

        KICData *kicData = KICCollection::KICParser(test_kicHeader, test_serverSendTime, test_boardSize, test_cleanDiagram);
        bool isPassed = assertKICData(kicData, expected_kicData);

        free(expectedServerSendTime);
        free(schedule0);
        free(schedule1);
        free(expected_kicData);
        free(kicData);

        return isPassed;
    }

    void runAllTests(){
        Serial.println("KICCollectionTestCase");
        Serial.printf(" - testKICLexer %s\n", testKICLexer() ? "passed" : "failed");
        Serial.printf(" - testKICParser %s\n", testKICParser() ? "passed" : "failed");
    }
};
