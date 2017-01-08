
#include "gtest/gtest.h"
#include "arduino-mock/Arduino.h" // Necessary to include for Serial
#include "arduino-mock/Serial.h"
#include "Smartcar.h" // The Smartcar library mock

#include "../../src/singleOdometer.ino" // Our production code

using ::testing::Return;
using ::testing::_;

class OdometerFixture : public ::testing::Test
{
public:
    ArduinoMock* arduinoMock; // Necessary for delay()
    SerialMock* serialMock;
    OdometerMock* odometerMock;
    // Run this before the tests
    virtual void SetUp()
    {
        arduinoMock = arduinoMockInstance();
        serialMock = serialMockInstance();
        odometerMock = odometerMockInstance();
    }
    // Run this after the tests
    virtual void TearDown()
    {
        releaseArduinoMock();
        releaseSerialMock();
        releaseOdometerMock();
    }
};

TEST_F(OdometerFixture, initsAreCalled) {
    EXPECT_CALL(*serialMock, begin(_));
    EXPECT_CALL(*odometerMock, attach(encoderPin));
    EXPECT_CALL(*odometerMock, begin());
    setup();
}

TEST_F(OdometerFixture, expectGetDistanceCall) {
    EXPECT_CALL(*odometerMock, getDistance());
    loop();
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
