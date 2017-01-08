
#include "gtest/gtest.h"
#include "arduino-mock/Arduino.h" // Necessary to include for Serial
#include "arduino-mock/Serial.h"
#include "Smartcar.h" // The Smartcar library mocks

#include "../../src/GP2D120_ir.ino" // Our production code

using ::testing::_;

class GP2D120Fixture : public ::testing::Test
{
public:
    ArduinoMock* arduinoMock; // Necessary for delay()
    SerialMock* serialMock;
    GP2D120Mock* GP2D120_mock;
    // Run this before the tests
    virtual void SetUp()
    {
        arduinoMock = arduinoMockInstance();
        serialMock = serialMockInstance();
        GP2D120_mock = GP2D120MockInstance();
    }
    // Run this after the tests
    virtual void TearDown()
    {
        releaseArduinoMock();
        releaseSerialMock();
        releaseGP2D120Mock();
    }
};

TEST_F(GP2D120Fixture, initsAreCalled) {
    EXPECT_CALL(*serialMock, begin(_));
    EXPECT_CALL(*GP2D120_mock, attach(SIDE_FRONT_PIN));
    setup();
}

TEST_F(GP2D120Fixture, expectGetDistanceCall) {
    EXPECT_CALL(*GP2D120_mock, getDistance());
    loop();
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
