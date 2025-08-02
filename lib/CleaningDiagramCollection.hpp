#ifndef _CLEANINGDIAGRAMCOLLECTION_
#define _CLEANINGDIAGRAMCOLLECTION_

#include <WString.h>

namespace CleaningDiagramCollection {
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
    bool CleaningDiagramParser(String& diagramString, CleaningDiagram *diagram);
    void freeDiagram(CleaningDiagram *diagram);

    // === Schedule ===
    DaySchedule ScheduleParser(String& scheduleString);
    bool setSchedule(DaySchedule schedule, CleaningDiagram diagram);
    void freeSchedule(char header, CleaningDiagram diagram);
}

#endif // _CLEANINGDIAGRAMCOLLECTION_
