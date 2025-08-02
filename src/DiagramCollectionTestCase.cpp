#include "../test/DiagramCollectionTestCase.hpp"
#include <HardwareSerial.h>

// KIC:V1;01437;01140334;008001200;20700090011001300;/

namespace DiagramCollectionTestCase {
    static DaySchedule* createSchedule(char header, unsigned int* hours, unsigned int length) {
        if (hours == nullptr || length == 0) {
            return nullptr;
        }

        DaySchedule* schedule = (DaySchedule*)calloc(1, sizeof(DaySchedule));
        if (schedule == nullptr) {
            return nullptr;
        }

        schedule->hours = (unsigned int*)calloc(length, sizeof(unsigned int));
        if (schedule->hours == nullptr) {
            free(schedule);
            return nullptr;
        }

        schedule->header = header;
        schedule->length = length;

        for (unsigned int i = 0; i < length; ++i) {
            schedule->hours[i] = hours[i];
        }

        return schedule;
    }

    static CleaningDiagram* createCleaningDiagram(DaySchedule** schedules, unsigned int scheduleCount) {
        if (schedules == nullptr || scheduleCount == 0) {
            return nullptr;
        }

        CleaningDiagram* diagram = (CleaningDiagram*)calloc(1, sizeof(CleaningDiagram));
        if (diagram == nullptr) {
            return nullptr;
        }

        diagram->schedules = (DaySchedule*)calloc(scheduleCount, sizeof(DaySchedule));
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

    static bool assertDaySchedule(const DaySchedule* a, const DaySchedule* b) {
        if (a == nullptr || b == nullptr) return false;
        if (a->header != b->header) return false;
        if (a->length != b->length) return false;

        for (unsigned int i = 0; i < a->length; ++i) {
            if (a->hours[i] != b->hours[i]) return false;
        }
        return true;
    }

    static bool assertCleaningDiagram(CleaningDiagram* diagram1, CleaningDiagram* diagram2) {
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
        String testDiagramString = "008001200;20700090011001300;";

        unsigned int emptyHours[] = {};
        DaySchedule* schedule0 = createSchedule('0', emptyHours, 0);
        DaySchedule* schedule1 = createSchedule('2', emptyHours, 0);

        schedule0->length = 2;
        schedule1->length = 4;
        schedule0->hours = (unsigned int*)calloc(2, sizeof(unsigned int));
        schedule1->hours = (unsigned int*)calloc(4, sizeof(unsigned int));

        DaySchedule* schedules[] = {schedule0, schedule1};
        CleaningDiagram* expected_diagram = createCleaningDiagram(schedules, 2);

        CleaningDiagram *diagram = ((void *)0);
        bool isSuccess = DiagramCollection::DiagramParser(testDiagramString, diagram);
        bool isPassed = assertCleaningDiagram(expected_diagram, diagram);

        freeDiagram(expected_diagram);
        freeDiagram(diagram);
        freeSchedule(schedule0);
        freeSchedule(schedule1);

        return isSuccess && isPassed;
    }

    bool testDiagramParserWithNONULL() {
        String testDiagramString = "008001200;20700090011001300;";

        unsigned int hours0[] = {800, 1200};
        unsigned int hours1[] = {700, 900, 1100, 1300};
        DaySchedule* expectedSchedule0 = createSchedule('0', hours0, 2);
        DaySchedule* expectedSchedule1 = createSchedule('2', hours1, 4);

        DaySchedule* expectedSchedules[] = {expectedSchedule0, expectedSchedule1};
        CleaningDiagram* expected_diagram = createCleaningDiagram(expectedSchedules, 2);

        unsigned int testHours0[] = {800, 1600};
        unsigned int testHours1[] = {700, 900, 1200, 1300};
        DaySchedule* testSchedule0 = createSchedule('0', testHours0, 2);
        DaySchedule* testSchedule1 = createSchedule('3', testHours1, 4);

        DaySchedule* testSchedules[] = {testSchedule0, testSchedule1};
        CleaningDiagram* diagram = createCleaningDiagram(testSchedules, 2);

        bool isSuccess = DiagramCollection::DiagramParser(testDiagramString, diagram);
        bool isPassed = assertCleaningDiagram(expected_diagram, diagram);

        freeDiagram(expected_diagram);
        freeDiagram(diagram);
        freeSchedule(expectedSchedule0);
        freeSchedule(expectedSchedule1);
        freeSchedule(testSchedule0);
        freeSchedule(testSchedule1);

        return isSuccess && isPassed;
    }

    bool testFreeDiagram() {
        unsigned int hours0[] = {800, 1200};
        unsigned int hours1[] = {700, 900, 1100, 1300};
        DaySchedule* schedule0 = createSchedule('0', hours0, 2);
        DaySchedule* schedule1 = createSchedule('3', hours1, 4);

        DaySchedule* schedules[] = {schedule0, schedule1};
        CleaningDiagram* test_diagram = createCleaningDiagram(schedules, 2);

        freeDiagram(test_diagram);
        bool isPassed = false;
        try {
            test_diagram->schedules[0].hours[0] = 0;
        }
        catch (const std::exception&) {
            isPassed = true;
        }

        freeSchedule(schedule0);
        freeSchedule(schedule1);
        return isPassed;
    }

    bool testScheduleParser() {
        String testString = "208001700";

        unsigned int expectedHours[] = {800, 1700};
        DaySchedule* expected_schedule = createSchedule('2', expectedHours, 2);

        DaySchedule* daySchedule = DiagramCollection::DiagramParser(testString);
        bool isPassed = assertDaySchedule(expected_schedule, daySchedule);

        freeSchedule(expected_schedule);
        freeSchedule(daySchedule);

        return isPassed;
    }

    bool testSetSchedule() {
        unsigned int hours0[] = {800, 1200};
        unsigned int hours1[] = {700, 900, 1100, 1300};
        DaySchedule* schedule0 = createSchedule('0', hours0, 2);
        DaySchedule* schedule1 = createSchedule('2', hours1, 4);

        DaySchedule* schedules[] = {schedule0, schedule1};
        CleaningDiagram* test_diagram = createCleaningDiagram(schedules, 2);

        unsigned int expectedHours0[] = {800, 1000, 1200, 1300};
        unsigned int expectedHours1[] = {700, 1100};
        DaySchedule* expectedSchedule0 = createSchedule('0', expectedHours0, 4);
        DaySchedule* expectedSchedule1 = createSchedule('2', expectedHours1, 2);

        DaySchedule* expectedSchedules[] = {expectedSchedule0, expectedSchedule1};
        CleaningDiagram* expected_diagram = createCleaningDiagram(expectedSchedules, 2);

        unsigned int newHours0[] = {800, 1000, 1200, 1300};
        unsigned int newHours1[] = {700, 1100};
        DaySchedule* newSchedule0 = createSchedule('0', newHours0, 4);
        DaySchedule* newSchedule1 = createSchedule('2', newHours1, 2);

        setSchedule(newSchedule0, test_diagram);
        setSchedule(newSchedule1, test_diagram);

        bool isPassed = assertCleaningDiagram(test_diagram, expected_diagram);

        freeDiagram(test_diagram);
        freeDiagram(expected_diagram);
        freeSchedule(schedule0);
        freeSchedule(schedule1);
        freeSchedule(expectedSchedule0);
        freeSchedule(expectedSchedule1);
        freeSchedule(newSchedule0);
        freeSchedule(newSchedule1);

        return isPassed;
    }

    bool testFreeSchedule() {
        unsigned int hours0[] = {800, 1200};
        unsigned int hours1[] = {700, 900, 1100, 1300};
        DaySchedule* schedule0 = createSchedule('0', hours0, 2);
        DaySchedule* schedule1 = createSchedule('2', hours1, 4);

        DaySchedule* schedules[] = {schedule0, schedule1};
        CleaningDiagram* test_diagram = createCleaningDiagram(schedules, 2);

        DaySchedule* expectedSchedules[] = {nullptr, schedule1};
        CleaningDiagram* expected_diagram = createCleaningDiagram(expectedSchedules, 2);

        setSchedule('0', test_diagram);
        setSchedule('1', test_diagram);

        bool isPassed = !assertCleaningDiagram(test_diagram, expected_diagram);

        freeDiagram(test_diagram);
        freeDiagram(expected_diagram);
        freeSchedule(schedule0);
        freeSchedule(schedule1);

        return isPassed;
    }

    void runAllTests() {
        Serial.println("DiagramCollectionTestCase");
        Serial.printf(" - testDiagramParserWithNULL %s\n", testDiagramParserWithNULL() ? "passed" : "failed");
        Serial.printf(" - testDiagramParserWithNONULL %s\n", testDiagramParserWithNONULL() ? "passed" : "failed");
        Serial.printf(" - testFreeDiagram %s\n", testFreeDiagram() ? "passed" : "failed");
        Serial.printf(" - testScheduleParser %s\n", testScheduleParser() ? "passed" : "failed");
        Serial.printf(" - testSetSchedule %s\n", testSetSchedule() ? "passed" : "failed");
        Serial.printf(" - testFreeSchedule %s\n", testFreeSchedule() ? "passed" : "failed");
    }
}
