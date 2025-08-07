#include "../lib/CleaningDiagramCollection.hpp" // WString.h, DaySchedule, CleaningDiagram
#include "../lib/KICCollection.hpp"
#include <HardwareSerial.h>

// 008001200;20700090011001300;/

// https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/charAt/

// === Diagram ===

/*
 * return true -> success, false -> falied
 * if diagram == NULL then set newDiagram
 * else then update diagram
 * */

// bool CleaningDiagramCollection::CleaningDiagramParser(const char*
// diagramString, CleaningDiagram& diagram) {
//     if(diagramString == nullptr) return false;
//
//     size_t len = strlen(diagramString);
//     size_t begin = 0, end = 0;
//     bool flagsToFree[7] = {true,true,true,true,true,true,true};
//
//     while (end < len && diagramString[end + 1] != KICCollection::KICEND) { //
//     LL(1)
//         // Find next segment separator
//         while (end < len && diagramString[end] !=
//         KICCollection::KICSEGMENTCHAR) end++;
//
//         // Extract substring manually
//         size_t substringLen = end - begin;
//         char* scheduleString = new char[substringLen + 1];
//         strncpy(scheduleString, diagramString + begin, substringLen);
//         scheduleString[substringLen] = '\0';
//
//         Weekday headerToWeekDay;
//         DaySchedule parsedSchedule;
//         bool isCurrectParsed = ScheduleParser(scheduleString,
//         headerToWeekDay, parsedSchedule); delete[] scheduleString; // Clean
//         up temporary string if(!isCurrectParsed) return false;
//
//         flagsToFree[(int)headerToWeekDay] = false;
//         diagram.schedules[(int)headerToWeekDay] = parsedSchedule;
//
//         begin = end + 1;
//         end = begin;
//     }
//
//     for(unsigned int header = 0; header < 7; header++)
//         if (flagsToFree[header]) initDaySchedule(diagram.schedules[header]);
//     return true;
// }

bool CleaningDiagramCollection::CleaningDiagramParser(
    const char *diagramString, CleaningDiagram &diagram) {

  if (diagramString == nullptr) {

    return false;
  }

  size_t len = strlen(diagramString);

  size_t begin = 0, end = 0;
  bool flagsToFree[7] = {true, true, true, true, true, true, true};

  int segmentCount = 0;

  while (end < len && diagramString[end] != KICCollection::KICEND) { // LL(1)

    // Find next segment separator
    while (end < len && diagramString[end] != KICCollection::KICSEGMENTCHAR) {
      end++;
    }

    // Extract substring manually
    size_t substringLen = end - begin;

    char *scheduleString = new char[substringLen + 1];
    strncpy(scheduleString, diagramString + begin, substringLen);
    scheduleString[substringLen] = '\0';

    Weekday headerToWeekDay;
    DaySchedule parsedSchedule;

    bool isCurrectParsed =
        ScheduleParser(scheduleString, headerToWeekDay, parsedSchedule);

    delete[] scheduleString; // Clean up temporary string

    if (!isCurrectParsed) {
      return false;
    }

    flagsToFree[(int)headerToWeekDay] = false;
    diagram.schedules[(int)headerToWeekDay] = parsedSchedule;

    begin = end + 1;
    end = begin;
    segmentCount++;
  }

  for (unsigned int header = 0; header < 7; header++) {
    if (flagsToFree[header]) {
      initDaySchedule(diagram.schedules[header]);
    }
  }

  return true;
}

// 008001200
// 20700090011001300
bool CleaningDiagramCollection::ScheduleParser(
    const char *scheduleString, CleaningDiagramCollection::Weekday &out_day,
    CleaningDiagramCollection::DaySchedule &out_daySchedule) {
  if (scheduleString == nullptr || scheduleString[0] == '\0')
    return false;

  DaySchedule parsedSchedule;
  initDaySchedule(parsedSchedule);

  // check scheduleString_length
  size_t strLen = strlen(scheduleString);
  size_t hasPayload = strLen - 1;
  size_t isNotDivisible = (strLen - 1) % DAYSCHEDULELENGTH;
  if (strLen == 0 || isNotDivisible || !hasPayload)
    return false;

  // extract header
  char c = scheduleString[0];
  if (c < '0' || '7' <= c)
    return false;
  out_day = (CleaningDiagramCollection::Weekday)(c - '0');

  // parse string
  size_t begin = 1;
  size_t end = begin + DAYSCHEDULELENGTH; // exclusive
  unsigned char scheduleAmount = (strLen - 1) / DAYSCHEDULELENGTH;
  char *segmentStr = new char[DAYSCHEDULELENGTH + 1];
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

void CleaningDiagramCollection::initDaySchedule(DaySchedule &ds) {
  for (int i = 0; i < MAX_HOURS_LENGTH; ++i)
    ds.hours[i] = INVAILHOUR;
  ds.length = 0;
}

void CleaningDiagramCollection::initCleaningDiagram(CleaningDiagram &cd) {
  for (int i = 0; i < AMOUNT_OF_DAY_INWEEK; ++i)
    initDaySchedule(cd.schedules[i]);
}
