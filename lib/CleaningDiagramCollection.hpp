#ifndef _CLEANINGDIAGRAMCOLLECTION_
#define _CLEANINGDIAGRAMCOLLECTION_

#include <WString.h>
#define AMOUNT_OF_DAY_INWEEK 7
#define MAX_HOURS_LENGTH 10

namespace CleaningDiagramCollection {
    inline constexpr unsigned int DAYSCHEDULELENGTH = 4; // 4
    inline constexpr short INVAILHOUR = -1;

    enum class Weekday : unsigned char {
        Sunday = 0,
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday
    };

    struct DaySchedule {
        short hours[MAX_HOURS_LENGTH];
        unsigned char length;
    };

    struct CleaningDiagram {
        DaySchedule schedules[AMOUNT_OF_DAY_INWEEK];
    };

    // === Diagram ===
    bool CleaningDiagramParser(const char* diagramString, CleaningDiagram& diagram);

    // === Schedule ===
    bool ScheduleParser(const char* scheduleString, CleaningDiagramCollection::Weekday& out_day, CleaningDiagramCollection::DaySchedule& out_daySchedule);
}

#endif // _CLEANINGDIAGRAMCOLLECTION_
