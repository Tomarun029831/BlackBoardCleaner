#include "../test/KICCollectionTestCase.hpp"
#include "../lib/KICCollection.hpp"
#include "../lib/CleaningDiagramCollection.hpp"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <WString.h>

namespace KICCollectionTestCase {
    static bool assertString(const char *str1, const char *str2) {
        if (str1 == nullptr || str2 == nullptr) return false;
        return strcmp(str1, str2) == 0;
    }

    static bool assertDaySchedule(const CleaningDiagramCollection::DaySchedule* a, const CleaningDiagramCollection::DaySchedule* b) {
        if (a == nullptr || b == nullptr) return false;
        // Remove header comparison as it doesn't exist in the struct
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
        for (unsigned int i = 0; i < AMOUNT_OF_DAY_INWEEK; ++i) {
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

        KICCollection::ServerTimestamp expectedServerTimestamp;
        expectedServerTimestamp.day = '0';
        expectedServerTimestamp.hour_minute = 1437;

        KICCollection::Board expectedBoard;
        expectedBoard.height = 144;
        expectedBoard.width = 334;

        short schedule0Hours[7] = {800, 1200, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR};
        CleaningDiagramCollection::DaySchedule schedule0;
        for(int i = 0; i < MAX_HOURS_LENGTH; i++){
            schedule0.hours[i] = schedule0Hours[i];
        }
short schedule1Hours[7] = {700, 900, 110, 1300, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR};
        CleaningDiagramCollection::DaySchedule schedule1;
        for(int i = 0; i < MAX_HOURS_LENGTH; i++){
            schedule1.hours[i] = schedule1Hours[i];
        }
        CleaningDiagramCollection::CleaningDiagram expected_diagram;
        for(int i = 0; i < MAX_HOURS_LENGTH; i++){
            expected_diagram.schedules[0].hours[i] = schedule0Hours[i];
        }
        expected_diagram.schedules[0].length = 2;

        for(int i = 0; i < MAX_HOURS_LENGTH; i++){
            expected_diagram.schedules[2].hours[i] = schedule1Hours[i];
        }
        expected_diagram.schedules[2].length = 4;
        
        KICCollection::KICData expected_kicData;
        expected_kicData.isNull = false;
        expected_kicData.serverTimestamp.day = expectedServerTimestamp.day;
        expected_kicData.serverTimestamp.hour_minute = expectedServerTimestamp.hour_minute;
        expected_kicData.board = expectedBoard;
        expected_kicData.diagram = expected_diagram;

        KICCollection::KICData kicData = KICCollection::KICParser(test_kicHeader, test_serverSendTime, test_boardSize, test_cleanDiagram);
        bool isPassed = kicData.isNull ? false : assertKICData(&kicData, &expected_kicData);

        return isPassed;
    }

    void runAllTests(){
        Serial.println("KICCollectionTestCase");
        Serial.printf(" - testKICLexer %s\n", testKICLexer() ? "passed" : "failed");
        Serial.printf(" - testKICParser %s\n", testKICParser() ? "passed" : "failed");
    }
};
