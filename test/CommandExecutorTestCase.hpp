#ifndef _COMMAND_EXECUTOR_TEST_CASE_HPP_
#define _COMMAND_EXECUTOR_TEST_CASE_HPP_

class CommandExecutorTestCase {
public:
    static void runAllTests();

private:
    static void testBasicCommands();
    static void testCustomCommands();
    static void testInvalidCommands();
};

#endif // _COMMAND_EXECUTOR_TEST_CASE_HPP_

