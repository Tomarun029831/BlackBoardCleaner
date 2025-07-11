#ifndef _MOTORMANUALONWALLTESTCASE_
#define _MOTORMANUALONWALLTESTCASE_

class MotorManualOnWallTestCase {
public:
    static void runAllTests();

private:
    static void testStopMovement();
    static void testFarwardMovement();
    static void testBackwardMovement();
    static void testRightRotation();
    static void testLeftRotation();
};

#endif // !_MOTORMANUALONWALLTESTCASE_
