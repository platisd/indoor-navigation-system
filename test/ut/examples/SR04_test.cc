#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "arduino-mock/Arduino.h"
#include "arduino-mock/Serial.h"
#include "Smartcar.h" // The Smartcar library mocks

#include "../../src/SR04.ino" // Our production code

using ::testing::_;

class SR04Fixture : public ::testing::Test
{
public:
    ArduinoMock* arduinoMock; // Necessary for delay()
    SerialMock* serialMock;
    SR04Mock* SR04_mock;
    // Run this before the tests
    virtual void SetUp()
    {
        arduinoMock = arduinoMockInstance();
        serialMock = serialMockInstance();
        SR04_mock = SR04MockInstance();
    }
    // Run this after the tests
    virtual void TearDown()
    {
        releaseArduinoMock();
        releaseSerialMock();
        releaseSR04Mock();
    }
};

TEST_F(SR04Fixture, initsAreCalled) {
    EXPECT_CALL(*serialMock, begin(_));
    EXPECT_CALL(*SR04_mock, attach(TRIGGER_PIN, ECHO_PIN));
    setup();
}

TEST_F(SR04Fixture, expectGetDistanceCall) {
    EXPECT_CALL(*SR04_mock, getMedianDistance(5));
    loop();
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
