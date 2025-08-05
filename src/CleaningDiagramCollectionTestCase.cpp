#include "../test/CleaningDiagramCollectionTestCase.hpp"
#include "../lib/CleaningDiagramCollection.hpp"
#include <HardwareSerial.h>

// KIC:V1;01437;01140334;008001200;20700090011001300;/

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

        short hours0[10] = {800, 1200, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR,CleaningDiagramCollection::INVAILHOUR,CleaningDiagramCollection::INVAILHOUR,CleaningDiagramCollection::INVAILHOUR};
        CleaningDiagramCollection::DaySchedule schedule0;
        for(int i = 0; i < MAX_HOURS_LENGTH; i++){
            schedule0.hours[i] = hours0[i];
        }
        schedule0.length = 2;
        unsigned int hours1[10] = {700, 900, 1100, 1300, CleaningDiagramCollection::INVAILHOUR,CleaningDiagramCollection::INVAILHOUR,CleaningDiagramCollection::INVAILHOUR};
        CleaningDiagramCollection::DaySchedule schedule1;
        for(int i = 0; i < MAX_HOURS_LENGTH; i++){
            schedule1.hours[i] = hours1[i];
        }
        schedule1.length = 4;
        CleaningDiagramCollection::DaySchedule schedules[2] = {schedule0, schedule1};
        CleaningDiagramCollection::CleaningDiagram expected_diagram;
        expected_diagram.schedules[0] = schedule0;
        expected_diagram.schedules[2] = schedule1;

        CleaningDiagramCollection::CleaningDiagram diagram;
        bool isSuccess = CleaningDiagramCollection::CleaningDiagramParser(testDiagramString, diagram);

        bool isPassed = assertCleaningDiagram(&expected_diagram, &diagram);

        return isSuccess && isPassed;
    }

    bool testDiagramParserWithNONULL() {
        const char* testDiagramString = "008001200;20700090011001300;";

        short expected_hours0[10] = {800, 1200, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR,CleaningDiagramCollection::INVAILHOUR,CleaningDiagramCollection::INVAILHOUR,CleaningDiagramCollection::INVAILHOUR,CleaningDiagramCollection::INVAILHOUR};
        CleaningDiagramCollection::DaySchedule expected_schedule0;
        for(int i = 0; i < MAX_HOURS_LENGTH; i++){
            expected_schedule0.hours[i] = expected_hours0[i];
        }
        expected_schedule0.length = 2;
        short expected_hours1[10] = {700, 900, 1100, 1300, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR};
        CleaningDiagramCollection::DaySchedule expected_schedule1;
        for(int i = 0; i < MAX_HOURS_LENGTH; i++){
            expected_schedule1.hours[i] = expected_hours1[i];
        }
        expected_schedule1.length = 4;
        CleaningDiagramCollection::CleaningDiagram expected_diagram;
        expected_diagram.schedules[0] = expected_schedule0;
        expected_diagram.schedules[2] = expected_schedule1;

        short hours0[10] = {800, 1600, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR};
        CleaningDiagramCollection::DaySchedule schedule0;
        for(int i = 0; i < MAX_HOURS_LENGTH; i++){
            schedule0.hours[i] = hours0[i];
        }
        short hours1[10] = {700, 900, 1200, 1300, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR, CleaningDiagramCollection::INVAILHOUR};
        CleaningDiagramCollection::DaySchedule schedule1;
        for(int i = 0; i < MAX_HOURS_LENGTH; i++){
            schedule1.hours[i] = hours1[i];
        } 
        schedule1.length = 4;
        CleaningDiagramCollection::CleaningDiagram diagram;
        diagram.schedules[0] = schedule0;
        diagram.schedules[2] = schedule1;

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

        // Test case 2: Header indicates 2 time slots but payload has only 1 time (too short)
        const char* testString1 = "20800"; 

        // Test case 3: Header indicates 2 time slots but payload has 3 times (too long)
        const char* testString2 = "2080017002000"; 

        // Test case 4: Invalid time format (hours > 24)
        const char* testString3 = "2250017000"; 

        // Test case 5: Empty string
        const char* testString4 = "";

        // Test case 6: Only header, no payload
        const char* testString5 = "2";

        CleaningDiagramCollection::Weekday test_weekday;
        CleaningDiagramCollection::DaySchedule parsedSchedule;

        // All these should return false (parsing failure)
        bool test0 = !CleaningDiagramCollection::ScheduleParser(testString0, test_weekday, parsedSchedule);
        bool test1 = !CleaningDiagramCollection::ScheduleParser(testString1, test_weekday, parsedSchedule);
        bool test2 = !CleaningDiagramCollection::ScheduleParser(testString2, test_weekday, parsedSchedule);
        bool test3 = !CleaningDiagramCollection::ScheduleParser(testString3, test_weekday, parsedSchedule);
        bool test4 = !CleaningDiagramCollection::ScheduleParser(testString4, test_weekday, parsedSchedule);
        bool test5 = !CleaningDiagramCollection::ScheduleParser(testString5, test_weekday, parsedSchedule);

        return test0 && test1 && test2 && test3 && test4 && test5;
    }

    void runAllTests() {
        Serial.println("DiagramCollectionTestCase");
        Serial.printf(" - testDiagramParserWithNULL %s\n", testDiagramParserWithNULL() ? "passed" : "failed");
        Serial.printf(" - testDiagramParserWithNONULL %s\n", testDiagramParserWithNONULL() ? "passed" : "failed");
        Serial.printf(" - testScheduleParserWithCurrentString %s\n", testScheduleParserWithCurrentString() ? "passed" : "failed");
        Serial.printf(" - testScheduleParserWithWrongString %s\n", testScheduleParserWithWrongString() ? "passed" : "failed");
    }
}
