#ifndef _MOTORMANUALONFLOORTESTCASE_
#define _MOTORMANUALONFLOORTESTCASE_

class MotorManualOnFloorTestCase {
public:
    static void setup();
    static void runAllTests();

private:
    static void testStopMovement();
    static void testFarwardMovement();
    static void testBackwardMovement();
    static void testRightRotation();
    static void testLeftRotation();
};

#endif // !_MOTORMANUALONFLOORTESTCASE_
