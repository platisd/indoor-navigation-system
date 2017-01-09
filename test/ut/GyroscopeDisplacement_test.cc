
#include "gtest/gtest.h"
#include "arduino-mock/Arduino.h" // Necessary to include for Serial
#include "arduino-mock/Serial.h"
#include "Smartcar.h" // The Smartcar library mocks

#include "../../src/gyroscopeDisplacement.ino" // Our production code

using ::testing::_;

class GyroscopeFixture : public ::testing::Test
{
public:
    ArduinoMock* arduinoMock; // Necessary for delay()
    SerialMock* serialMock;
    GyroscopeMock* gyroscopeMock;
    // Run this before the tests
    virtual void SetUp()
    {
        arduinoMock = arduinoMockInstance();
        serialMock = serialMockInstance();
        gyroscopeMock = gyroscopeMockInstance();
    }
    // Run this after the tests
    virtual void TearDown()
    {
        releaseArduinoMock();
        releaseSerialMock();
        releaseGyroscopeMock();
    }
};

TEST_F(GyroscopeFixture, initsAreCalled) {
    EXPECT_CALL(*serialMock, begin(_));
    EXPECT_CALL(*gyroscopeMock, begin(_));
    setup();
}

TEST_F(GyroscopeFixture, updateCalledForAngularDisplacementToBeCalculated) {
    EXPECT_CALL(*gyroscopeMock, update());
    EXPECT_CALL(*gyroscopeMock, getAngularDisplacement());
    loop();
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
