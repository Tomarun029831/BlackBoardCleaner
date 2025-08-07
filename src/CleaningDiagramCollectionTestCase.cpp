#include "../test/CleaningDiagramCollectionTestCase.hpp"
#include "../lib/CleaningDiagramCollection.hpp"
#include <HardwareSerial.h>

// KIC:V3;01437;01140334;008001200;20700090011001300;/

namespace CleaningDiagramCollectionTestCase {
    static bool assertDaySchedule(const CleaningDiagramCollection::DaySchedule* a, const CleaningDiagramCollection::DaySchedule* b) {
        if (a == nullptr || b == nullptr) return false;
        // Remove header comparison as it doesn't exist in the struct
        if (a->length != b->length) return false;

        for (unsigned int i = 0; i < a->length; ++i) {
            if (a->hours[i] != b->hours[i]) return false;
        }
        return true;
    }

    static bool assertCleaningDiagram(const CleaningDiagramCollection::CleaningDiagram* diagram1, const CleaningDiagramCollection::CleaningDiagram* diagram2) {
        if (diagram1 == nullptr || diagram2 == nullptr) return false;
        // CleaningDiagram doesn't have length member, so compare all 7 days (AMOUNT_OF_DAY_INWEEK)
        for (unsigned int i = 0; i < AMOUNT_OF_DAY_INWEEK; ++i) {
            if (!assertDaySchedule(&diagram1->schedules[i], &diagram2->schedules[i])) {
                return false;
            }
        }
        return true;
    }

    bool testDiagramParserWithNULL() {
        const char* testDiagramString = "008001200;20700090011001300;";

        CleaningDiagramCollection::DaySchedule schedule0;
        initDaySchedule(schedule0);
        schedule0.hours[0] = 800;
        schedule0.hours[1] = 1200;
        schedule0.length = 2;

        short hours1[10] = {700, 900, 1100, 1300, CleaningDiagramCollection::INVAILHOUR,CleaningDiagramCollection::INVAILHOUR,CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR,CleaningDiagramCollection::INVAILHOUR,CleaningDiagramCollection::INVAILHOUR};
        CleaningDiagramCollection::DaySchedule schedule1;
        initDaySchedule(schedule1);
        schedule1.hours[0] = 700;
        schedule1.hours[1] = 900;
        schedule1.hours[2] = 1100;
        schedule1.hours[3] = 1300;
        schedule1.length = 4;
        CleaningDiagramCollection::CleaningDiagram expected_diagram;
        initCleaningDiagram(expected_diagram);
        expected_diagram.schedules[0] = schedule0;
        expected_diagram.schedules[2] = schedule1;

        CleaningDiagramCollection::CleaningDiagram diagram;
        initCleaningDiagram(diagram);
        bool isSuccess = CleaningDiagramCollection::CleaningDiagramParser(testDiagramString, diagram);

        bool isPassed = assertCleaningDiagram(&expected_diagram, &diagram);

        return isSuccess && isPassed;
    }

    bool testDiagramParserWithNONULL() {
        const char* testDiagramString = "008001200;20700090011001300;";

        CleaningDiagramCollection::DaySchedule expected_schedule0;
        initDaySchedule(expected_schedule0);
        expected_schedule0.hours[0] = 800;
        expected_schedule0.hours[1] = 1200;
        expected_schedule0.length = 2;

        CleaningDiagramCollection::DaySchedule expected_schedule1;
        initDaySchedule(expected_schedule1);
        expected_schedule1.hours[0] = 700;
        expected_schedule1.hours[1] = 900;
        expected_schedule1.hours[2] = 1100;
        expected_schedule1.hours[3] = 1300;
        expected_schedule1.length = 4;

        CleaningDiagramCollection::CleaningDiagram expected_diagram;
        initCleaningDiagram(expected_diagram);
        expected_diagram.schedules[0] = expected_schedule0;
        expected_diagram.schedules[2] = expected_schedule1;

        CleaningDiagramCollection::DaySchedule schedule0;
        initDaySchedule(schedule0);
        schedule0.hours[0] = 800;
        schedule0.hours[1] = 1600;

        CleaningDiagramCollection::DaySchedule schedule1;
        initDaySchedule(schedule1);
        schedule1.hours[0] = 700;
        schedule1.hours[1] = 900;
        schedule1.hours[2] = 1200;
        schedule1.hours[3] = 1300;
        schedule1.length = 4;

        CleaningDiagramCollection::CleaningDiagram diagram;
        initCleaningDiagram(diagram);
        diagram.schedules[0] = schedule0;
        diagram.schedules[3] = schedule1;

        bool isSuccess = CleaningDiagramCollection::CleaningDiagramParser(testDiagramString, diagram);
        bool isPassed = assertCleaningDiagram(&expected_diagram, &diagram);

        return isSuccess && isPassed;
    }

    bool testScheduleParserWithCurrentString() { 
        const char* testString = "208001700"; 

        unsigned int hours[10] = {800, 1700, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR}; 
        CleaningDiagramCollection::DaySchedule expected_schedule;
        for(int i = 0; i < MAX_HOURS_LENGTH; i++){
            expected_schedule.hours[i] = hours[i];
        }
        expected_schedule.length = 2;
        CleaningDiagramCollection::Weekday expected_weekday = CleaningDiagramCollection::Weekday::Tuesday; 

        CleaningDiagramCollection::Weekday test_weekday; 
        CleaningDiagramCollection::DaySchedule parsedSchedule; 
        bool isSuccess = CleaningDiagramCollection::ScheduleParser(testString, test_weekday, parsedSchedule); 
        bool isPassed = isSuccess && assertDaySchedule(&expected_schedule, &parsedSchedule) && (test_weekday == expected_weekday); 

        return isPassed; 
    }

    bool testScheduleParserWithWrongString() { 
        // Test case 1: Invalid header (7 is not a valid weekday)
        const char* testString0 = "708001700"; 

        // Test case 2: Invalid time format (hours > 24)
        const char* testString1 = "2250017000"; 

        // Test case 3: Empty string
        const char* testString2 = "";

        // Test case 4: Only header, no payload
        const char* testString3 = "2";

        CleaningDiagramCollection::Weekday test_weekday;
        CleaningDiagramCollection::DaySchedule parsedSchedule;

        // All these should return false (parsing failure)
        bool test0 = !CleaningDiagramCollection::ScheduleParser(testString0, test_weekday, parsedSchedule);
        bool test1 = !CleaningDiagramCollection::ScheduleParser(testString1, test_weekday, parsedSchedule);
        bool test2 = !CleaningDiagramCollection::ScheduleParser(testString2, test_weekday, parsedSchedule);
        bool test3 = !CleaningDiagramCollection::ScheduleParser(testString3, test_weekday, parsedSchedule);

        return test0 && test1 && test2 && test3;
    }

    void runAllTests() {
        Serial.println("DiagramCollectionTestCase");
        Serial.printf(" - testDiagramParserWithNULL %s\n", testDiagramParserWithNULL() ? "passed" : "failed");
        Serial.printf(" - testDiagramParserWithNONULL %s\n", testDiagramParserWithNONULL() ? "passed" : "failed");
        Serial.printf(" - testScheduleParserWithCurrentString %s\n", testScheduleParserWithCurrentString() ? "passed" : "failed");
        Serial.printf(" - testScheduleParserWithWrongString %s\n", testScheduleParserWithWrongString() ? "passed" : "failed");
    }
}
