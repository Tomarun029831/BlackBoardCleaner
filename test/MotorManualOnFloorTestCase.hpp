#ifndef MOTOR_MANUAL_ON_FLOOR_TEST_CASE_HPP
#define MOTOR_MANUAL_ON_FLOOR_TEST_CASE_HPP

class MotorManualOnFloorTestCase {
public:
    // Setup function
    static void setup();
    
    // Individual test functions
    static void testStopMovement();
    static void testForwardMovement();
    static void testBackwardMovement();
    static void testRightRotation();
    static void testLeftRotation();
    static void testPinStatus();  // 新しいテスト関数
    
    // Test suite functions
    static void runBasicMovementTest();     // Basic forward movement only
    static void runFullMovementTest();      // All movement types
    static void runSquarePathTest();        // Complex path test
    static void runDiagnosticTest();        // Pin状態診断テスト
    static void runAllTests();              // Main test runner
};

#endif // MOTOR_MANUAL_ON_FLOOR_TEST_CASE_HPP
