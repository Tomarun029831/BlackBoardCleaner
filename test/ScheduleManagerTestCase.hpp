#ifndef _SCHEDULE_MANAGER_TEST_CASE_HPP_
#define _SCHEDULE_MANAGER_TEST_CASE_HPP_

class ScheduleManagerTestCase {
public:
    static void runAllTests();

private:
    static void testAddSchedule();
    static void testRemoveSchedule();
    static void testCheckPendingCommands();
    static void testTimeManagement();
};

#endif // _SCHEDULE_MANAGER_TEST_CASE_HPP_

