#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "arduino-mock/Arduino.h"
#include "arduino-mock/Serial.h"
#include "Smartcar.h" // The Smartcar library mocks

#include "../../src/SRF08.ino" // Our production code

using ::testing::_;

class SRF08Fixture : public ::testing::Test
{
public:
    ArduinoMock* arduinoMock; // Necessary for delay()
    SerialMock* serialMock;
    SRF08Mock* SRF08_mock;
    // Run this before the tests
    virtual void SetUp()
    {
        arduinoMock = arduinoMockInstance();
        serialMock = serialMockInstance();
        SRF08_mock = SRF08MockInstance();
    }
    // Run this after the tests
    virtual void TearDown()
    {
        releaseArduinoMock();
        releaseSerialMock();
        releaseSRF08Mock();
    }
};

TEST_F(SRF08Fixture, initsAreCalled) {
    EXPECT_CALL(*serialMock, begin(_));
    EXPECT_CALL(*SRF08_mock, attach(112));
    EXPECT_CALL(*SRF08_mock, setGain(GAIN));
    EXPECT_CALL(*SRF08_mock, setRange(RANGE));
    EXPECT_CALL(*SRF08_mock, setPingDelay(8));
    setup();
}

TEST_F(SRF08Fixture, expectGetDistanceCall) {
    EXPECT_CALL(*SRF08_mock, getDistance());
    loop();
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
