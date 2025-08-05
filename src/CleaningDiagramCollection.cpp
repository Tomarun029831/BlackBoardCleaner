#include "../lib/CleaningDiagramCollection.hpp" // WString.h, DaySchedule, CleaningDiagram
#include "../lib/KICCollection.hpp"

// 008001200;20700090011001300;/

// https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/charAt/

// === Diagram ===

/*
 * return true -> success, false -> falied
 * if diagram == NULL then set newDiagram
 * else then update diagram
 * */
bool CleaningDiagramCollection::CleaningDiagramParser(const char* diagramString, CleaningDiagram& diagram) {
    if(diagramString == nullptr) return false;

    size_t len = strlen(diagramString);
    size_t begin = 0, end = 0;
    bool flagsToFree[7] = {true};

    while (end < len && diagramString[end + 1] != KICCollection::KICEND) { // LL(1)
        // Find next segment separator
        while (end < len && diagramString[end] != KICCollection::KICSEGMENTCHAR) end++;

        // Extract substring manually
        size_t substringLen = end - begin;
        char* scheduleString = new char[substringLen + 1];
        strncpy(scheduleString, diagramString + begin, substringLen);
        scheduleString[substringLen] = '\0';

        Weekday headerToWeekDay;
        DaySchedule parsedSchedule;
        bool isCurrectParsed = ScheduleParser(scheduleString, headerToWeekDay, parsedSchedule);
        delete[] scheduleString; // Clean up temporary string
        if(!isCurrectParsed) return false;

        flagsToFree[(int)headerToWeekDay] = false;
        diagram.schedules[(int)headerToWeekDay] = parsedSchedule;

        begin = end + 1;
        end = begin;
    }

    for(unsigned int header = 0; header < 7; header++)
        if (flagsToFree[header]) diagram.schedules[header].length = 0;
    return true;
}

// 008001200
// 20700090011001300
bool CleaningDiagramCollection::ScheduleParser(const char* scheduleString, CleaningDiagramCollection::Weekday& out_day, CleaningDiagramCollection::DaySchedule& out_daySchedule) {
    DaySchedule parsedSchedule;
    for (int i = 0; i < MAX_HOURS_LENGTH; i++) {
        parsedSchedule.hours[i] = INVAILHOUR;
    }
    parsedSchedule.length = 0;

    if (scheduleString == nullptr || scheduleString == "") return false;

    // check scheduleString_length
    size_t strLen = strlen(scheduleString);
    size_t isNotDivisible = (strLen - 1) % DAYSCHEDULELENGTH;
    if (strLen == 0 || isNotDivisible) return false;

    // extract header
    char c = scheduleString[0];
    if (c > '0' || '7' < c) return false;
    out_day = (CleaningDiagramCollection::Weekday)(c - '0');

    // parse string
    size_t begin = 1;
    size_t end = begin + DAYSCHEDULELENGTH; // exclusive
    unsigned char scheduleAmount = (strLen - 1) / DAYSCHEDULELENGTH;
    char* segmentStr = new char[DAYSCHEDULELENGTH + 1];
    for (unsigned int i = 0; i < scheduleAmount; i++) {
        strncpy(segmentStr, scheduleString + begin, DAYSCHEDULELENGTH);
        segmentStr[DAYSCHEDULELENGTH] = '\0';

        parsedSchedule.hours[i] = (short)atoi(segmentStr);

        begin = end;
        end = begin + DAYSCHEDULELENGTH;
    }
    delete[] segmentStr; // Clean up temporary string
    parsedSchedule.length = scheduleAmount;
    out_daySchedule = parsedSchedule;

    return true;
}
