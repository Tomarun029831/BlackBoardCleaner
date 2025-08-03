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
bool CleaningDiagramCollection::CleaningDiagramParser(String& diagramString, CleaningDiagram *diagram) {
    if(diagramString == nullptr) return false;
    unsigned int begin = 0, end = 0;
    bool flagsToFree[7] = {true};
    if (diagram == nullptr) diagram = (CleaningDiagram *)calloc(1, sizeof(CleaningDiagram));

    while (diagramString.charAt(end + 1) != KICCollection::KICEND) { // LL(1)
        end = diagramString.indexOf(KICCollection::KICSEGMENTCHAR, begin);
        String scheduleString = diagramString.substring(begin, end);
        DaySchedule schedule = ScheduleParser(scheduleString);
        if (schedule.length == 0) return false;

        unsigned int headerToInt = schedule.header - '0';
        flagsToFree[headerToInt] = {false};

        bool isSuccess = setSchedule(schedule, *diagram);
        if (!isSuccess) return false;
    }

    for(unsigned int header = 0; header < 7; header++)
        if (flagsToFree[header]) deleteSchedule(header, *diagram);

    return true;
}

void CleaningDiagramCollection::freeDiagram(CleaningDiagram *diagram) {
    freeSchedule(diagram->schedules);
    diagram->schedules = nullptr;
    free(diagram);
}

// === Schedule ===

// 008001200
// 20700090011001300
CleaningDiagramCollection::DaySchedule CleaningDiagramCollection::ScheduleParser(String& scheduleString) {
    DaySchedule parsedSchedule = {'\0', nullptr, 0};
    unsigned int begin = 1, end = begin + KICCollection::DAYSCHEDULELENGTH;

    unsigned int scheduleAmount = (scheduleString.length() - 1) / KICCollection::DAYSCHEDULELENGTH; // str_length - header_length
    // extract header
    parsedSchedule.header = scheduleString.charAt(0);
    // allocate schedules
    parsedSchedule.hours = (unsigned int *)calloc(scheduleAmount, sizeof(unsigned int));
    parsedSchedule.length = scheduleAmount;
    // parse String
    for (unsigned int i = 0; i < scheduleAmount; i++) {
        String segmentStr = scheduleString.substring(begin, end);
        parsedSchedule.hours[i] = (unsigned int)segmentStr.toInt();
    }

    return parsedSchedule;
}

void CleaningDiagramCollection::freeSchedule(DaySchedule *schedule) {
    free(schedule->hours);
    free(schedule);
}

bool CleaningDiagramCollection::setSchedule(DaySchedule schedule, CleaningDiagram& diagram) {
    bool isWritten = false;
    for(unsigned int i = 0; i < diagram.length; i++){ // PERF: O(n)
        if(diagram.schedules[i].header == schedule.header) {
            diagram.schedules[i] = schedule;
            isWritten = true;
        }
    }

    if(isWritten) return true;

    DaySchedule *newPtr = (DaySchedule *)realloc(diagram.schedules, (diagram.length + 1) * sizeof(DaySchedule));
    if(newPtr == ((void *)0)) return false;
    diagram.schedules = newPtr;

    diagram.schedules[diagram.length++] = schedule;
    return true;
}

void CleaningDiagramCollection::deleteSchedule(char header, CleaningDiagram& diagram) {
    unsigned int headerToInt = header - '0';
    unsigned int writeIndex = 0;

    for (unsigned int readIndex = 0; readIndex < diagram.length; readIndex++) { // PERF: O(n)
        if (diagram.schedules[readIndex].header == headerToInt) continue;
        diagram.schedules[writeIndex++] = diagram.schedules[readIndex];
    }

    diagram.length = writeIndex;
}

