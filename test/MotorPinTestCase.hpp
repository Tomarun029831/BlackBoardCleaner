#ifndef _MOTORPINTESTCASE_
#define _MOTORPINTESTCASE_

class MotorPinTestCase {
public:
    static void runAllTests();

private:
    static void testStopSignal();
    static void testForwardSignal();
    static void testBackwardSignal();
    static void testRightRotationSignal();
    static void testLeftRotationSignal();
};

#endif // !_MOTORPINTESTCASE_
