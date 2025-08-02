#ifndef _SCHEDULECOLLECTION_
#define _SCHEDULECOLLECTION_

#include <WString.h>

namespace DiagramCollection {
    struct DaySchedule {
        char header;
        unsigned int hours*;
        unsigned int length;
    };

    struct CleaningDiagram {
        DaySchedule *schedules;
        unsigned int length;
    };

    // === Diagram ===

    /*
     * if diagram == NULL then set newDiagram
     * else then update diagram
     * */
    bool DiagramParser(String& diagramString, CleaningDiagram *diagram);
    void freeDiagram(CleaningDiagram *diagram);

    // === Schedule ===
    DaySchedule *ScheduleParser(String& scheduleString);
    bool setSchedule(DaySchedule schedule, CleaningDiagram diagram);
    void freeSchedule(char header, CleaningDiagram diagram);
}

#endif // _SCHEDULECOLLECTION_
