#include "../test/CleaningDiagramCollectionTestCase.hpp"
#include "../lib/CleaningDiagramCollection.hpp"
#include <HardwareSerial.h>

// KIC:V1;01437;01140334;008001200;20700090011001300;/

namespace CleaningDiagramCollectionTestCase {
    static CleaningDiagramCollection::DaySchedule* createSchedule(char header, unsigned int* hours, unsigned int length) {
        CleaningDiagramCollection::DaySchedule* schedule = (CleaningDiagramCollection::DaySchedule*)calloc(1, sizeof(CleaningDiagramCollection::DaySchedule));
        if (schedule == nullptr) {
            return nullptr;
        }

        schedule->header = header;
        schedule->length = length;

        if (length > 0) {
            if (hours == nullptr) {
                free(schedule);
                return nullptr;  // hours がnullなのはエラー
            }

            schedule->hours = (unsigned int*)calloc(length, sizeof(unsigned int));
            if (schedule->hours == nullptr) {
                free(schedule);
                return nullptr;
            }

            for (unsigned int i = 0; i < length; ++i) {
                schedule->hours[i] = hours[i];
            }
        } else {
            schedule->hours = nullptr;  // length 0 の場合はhoursもnullptrでOK
        }

        return schedule;
    }

    static CleaningDiagramCollection::CleaningDiagram* createCleaningDiagram(CleaningDiagramCollection::DaySchedule** schedules, unsigned int scheduleCount) {
        if (schedules == nullptr || scheduleCount == 0) {
            return nullptr;
        }

        CleaningDiagramCollection::CleaningDiagram* diagram = (CleaningDiagramCollection::CleaningDiagram*)calloc(1, sizeof(CleaningDiagramCollection::CleaningDiagram));
        if (diagram == nullptr) {
            return nullptr;
        }

        diagram->schedules = (CleaningDiagramCollection::DaySchedule*)calloc(scheduleCount, sizeof(CleaningDiagramCollection::DaySchedule));
        if (diagram->schedules == nullptr) {
            free(diagram);
            return nullptr;
        }

        diagram->length = scheduleCount;

        for (unsigned int i = 0; i < scheduleCount; ++i) {
            if (schedules[i] != nullptr) {
                diagram->schedules[i] = *schedules[i];
                diagram->schedules[i].hours = (unsigned int*)calloc(schedules[i]->length, sizeof(unsigned int));
                if (diagram->schedules[i].hours != nullptr) {
                    for (unsigned int j = 0; j < schedules[i]->length; ++j) {
                        diagram->schedules[i].hours[j] = schedules[i]->hours[j];
                    }
                }
            }
        }

        return diagram;
    }

    static bool assertDaySchedule(const CleaningDiagramCollection::DaySchedule* a, const CleaningDiagramCollection::DaySchedule* b) {
        if (a == nullptr || b == nullptr) return false;
        if (a->header != b->header) return false;
        if (a->length != b->length) return false;

        for (unsigned int i = 0; i < a->length; ++i) {
            if (a->hours[i] != b->hours[i]) return false;
        }
        return true;
    }

    static bool assertCleaningDiagram(CleaningDiagramCollection::CleaningDiagram* diagram1, CleaningDiagramCollection::CleaningDiagram* diagram2) {
        if (diagram1 == nullptr || diagram2 == nullptr) return false;
        if (diagram1->length != diagram2->length) return false;

        for (unsigned int i = 0; i < diagram1->length; ++i) {
            if (!assertDaySchedule(&diagram1->schedules[i], &diagram2->schedules[i])) {
                return false;
            }
        }

        return true;
    }

    bool testDiagramParserWithNULL() {
        const char* testDiagramString = "008001200;20700090011001300;";

        unsigned int hours0[2] = {800, 1200};
        CleaningDiagramCollection::DaySchedule schedule0 = {'0', hours0, 2};
        unsigned int hours1[4] = {700, 900, 1100, 1300};
        CleaningDiagramCollection::DaySchedule schedule1 = {'2', hours1, 4};
        CleaningDiagramCollection::DaySchedule schedules[2] = {schedule0, schedule1};
        CleaningDiagramCollection::CleaningDiagram expected_diagram = {schedules, 2};

        CleaningDiagramCollection::CleaningDiagram diagram = {nullptr, 0};
        bool isSuccess = CleaningDiagramCollection::CleaningDiagramParser(testDiagramString, diagram);

        bool isPassed = assertCleaningDiagram(&expected_diagram, &diagram);

        return isSuccess && isPassed;
    }

    bool testDiagramParserWithNONULL() {
        const char* testDiagramString = "008001200;20700090011001300;";

        unsigned int expected_hours0[2] = {800, 1200};
        CleaningDiagramCollection::DaySchedule expected_schedule0 = {'0', expected_hours0, 2};
        unsigned int expected_hours1[4] = {700, 900, 1100, 1300};
        CleaningDiagramCollection::DaySchedule expected_schedule1 = {'2', expected_hours1, 4};
        CleaningDiagramCollection::DaySchedule expected_schedules[2] = {expected_schedule0, expected_schedule1};
        CleaningDiagramCollection::CleaningDiagram expected_diagram = {expected_schedules , 2};

        unsigned int hours0[2] = {800, 1600};
        CleaningDiagramCollection::DaySchedule schedule0 = {'0', hours0, 2};
        unsigned int hours1[4] = {700, 900, 1200, 1300};
        CleaningDiagramCollection::DaySchedule schedule1 = {'3', hours1, 4};
        CleaningDiagramCollection::DaySchedule schedules[2] = {schedule0, schedule1};
        CleaningDiagramCollection::CleaningDiagram diagram = {schedules, 2};

        bool isSuccess = CleaningDiagramCollection::CleaningDiagramParser(testDiagramString, diagram);
        bool isPassed = assertCleaningDiagram(&expected_diagram, &diagram);

        return isSuccess && isPassed;
    }

    bool testScheduleParser() {
        const char* testString = "208001700";

        unsigned int hours[2] = {800, 1700};
        CleaningDiagramCollection::DaySchedule expected_schedule = {'2', hours, 2};

        CleaningDiagramCollection::DaySchedule parsedSchedule = CleaningDiagramCollection::ScheduleParser(testString);
        bool isPassed = assertDaySchedule(&expected_schedule, &parsedSchedule);

        return isPassed;
    }

    bool testFreeSchedule() {
        bool isPassed = false;
        CleaningDiagramCollection::DaySchedule *schedules = (CleaningDiagramCollection::DaySchedule *)calloc(3, sizeof(CleaningDiagramCollection::DaySchedule));
        schedules[0].header = '0';
        schedules[0].hours = (unsigned int *)calloc(2, sizeof(CleaningDiagramCollection::DaySchedule));
        schedules[0].hours[0] = 800;
        schedules[0].hours[1] = 1200;
        schedules[0].length = 2;

        schedules[1].header = '1';
        schedules[1].hours = (unsigned int *)calloc(3, sizeof(CleaningDiagramCollection::DaySchedule));
        schedules[1].hours[0] = 100;
        schedules[1].hours[1] = 2300;
        schedules[1].hours[2] = 1300;
        schedules[1].length = 3;
 
        schedules[2].header = '2';
        schedules[2].hours = (unsigned int *)calloc(2, sizeof(CleaningDiagramCollection::DaySchedule));
        schedules[2].hours[0] = 1400;
        schedules[2].hours[1] = 1555;
        schedules[2].length = 2;

        unsigned int *checkPtr = schedules[0].hours;

        freeSchedule(schedules);

        try {
            *checkPtr = 100;
        }
        catch (const std::exception&) {
            isPassed = true;
            return isPassed;
        }

        return isPassed;
    }

    bool testSetSchedule() {
        unsigned int hours0[] = {800, 1200};
        CleaningDiagramCollection::DaySchedule schedule0 = {'0', hours0, 2};
        unsigned int hours1[] = {700, 900, 1100, 1300};
        CleaningDiagramCollection::DaySchedule schedule1 = {'2', hours1, 4};
        CleaningDiagramCollection::DaySchedule schedules[] = {schedule0, schedule1};
        CleaningDiagramCollection::CleaningDiagram test_diagram = {schedules, 2};

        unsigned int expectedHours0[] = {800, 1000, 1200, 1300};
        CleaningDiagramCollection::DaySchedule expectedSchedule0 = {'0', expectedHours0, 4};
        unsigned int expectedHours1[] = {700, 1100};
        CleaningDiagramCollection::DaySchedule expectedSchedule1 = {'2', expectedHours1, 2};
        CleaningDiagramCollection::DaySchedule expectedSchedules[] = {expectedSchedule0, expectedSchedule1};
        CleaningDiagramCollection::CleaningDiagram expected_diagram = {expectedSchedules, 2};

        unsigned int newHours0[] = {800, 1000, 1200, 1300};
        CleaningDiagramCollection::DaySchedule newSchedule0 = {'0', newHours0, 4};
        unsigned int newHours1[] = {700, 1100};
        CleaningDiagramCollection::DaySchedule newSchedule1 = {'2', newHours1, 2};

        setSchedule(newSchedule0, test_diagram);
        setSchedule(newSchedule1, test_diagram);

        bool isPassed = assertCleaningDiagram(&test_diagram, &expected_diagram);

        return isPassed;
    }

    bool testDeleteSchedule() {
        unsigned int hours0[] = {800, 1200};
        CleaningDiagramCollection::DaySchedule schedule0 = {'0', hours0, 2};
        unsigned int hours1[] = {700, 900, 1100, 1300};
        CleaningDiagramCollection::DaySchedule schedule1 = {'2', hours1, 4};
        CleaningDiagramCollection::DaySchedule schedules[] = {schedule0, schedule1};
        CleaningDiagramCollection::CleaningDiagram test_diagram = {schedules, 2};

        unsigned int expected_hours0[] = {700, 900, 1100, 1300};
        CleaningDiagramCollection::DaySchedule expected_schedule0 = {'2', expected_hours0, 4};
        CleaningDiagramCollection::DaySchedule expectedSchedules[] = {expected_schedule0};
        CleaningDiagramCollection::CleaningDiagram expected_diagram = {expectedSchedules, 1};

        deleteSchedule('0', test_diagram);
        deleteSchedule('1', test_diagram);

        bool isPassed = assertCleaningDiagram(&test_diagram, &expected_diagram);

        return isPassed;
    }

    void runAllTests() {
        Serial.println("DiagramCollectionTestCase");
        Serial.printf(" - testDiagramParserWithNULL %s\n", testDiagramParserWithNULL() ? "passed" : "failed");
        Serial.printf(" - testDiagramParserWithNONULL %s\n", testDiagramParserWithNONULL() ? "passed" : "failed");
        Serial.printf(" - testScheduleParser %s\n", testScheduleParser() ? "passed" : "failed");
        Serial.printf(" - testSetSchedule %s\n", testSetSchedule() ? "passed" : "failed");
        Serial.printf(" - testDeleteSchedule %s\n", testDeleteSchedule() ? "passed" : "failed");
    }
}
