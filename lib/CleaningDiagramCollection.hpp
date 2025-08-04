#ifndef _CLEANINGDIAGRAMCOLLECTION_
#define _CLEANINGDIAGRAMCOLLECTION_

#include <WString.h>

namespace CleaningDiagramCollection {
    struct DaySchedule {
        char header;
        unsigned int *hours;
        unsigned int length;
    };

    struct CleaningDiagram {
        DaySchedule *schedules;
        unsigned int length;
    };

    // === Diagram ===
    bool CleaningDiagramParser(const char* diagramString, CleaningDiagram& diagram);

    // === Schedule ===
    DaySchedule ScheduleParser(const char* scheduleString);
    void freeSchedule(DaySchedule *schedule);
    bool setSchedule(DaySchedule schedule, CleaningDiagram& diagram);
    void deleteSchedule(char header, CleaningDiagram& diagram);
}

#endif // _CLEANINGDIAGRAMCOLLECTION_
