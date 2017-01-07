// Copyright 2014 http://switchdevice.com
// This example code is in the public domain.

#include "gtest/gtest.h"
#include "arduino-mock/Arduino.h"
#include "arduino-mock/Serial.h"
#include "Smartcar.h"

#include "../../src/manualControl.ino"

using ::testing::Return;
using ::testing::_;

class SmartcarManualControlFixture : public ::testing::Test
{
   public:
     ArduinoMock* arduinoMock;
     SerialMock* serialMock;
     CarMock* carMock;
    // Run this before the tests
    virtual void SetUp()
    {
      arduinoMock = arduinoMockInstance();
      serialMock = serialMockInstance();
      carMock = carMockInstance();
    }
    // Run this after the tests
    virtual void TearDown()
    {
      releaseSerialMock();
      releaseArduinoMock();
      releaseCarMock();
    }
};

TEST_F(SmartcarManualControlFixture, initsAreCalled) {
    EXPECT_CALL(*serialMock, begin(_));
    EXPECT_CALL(*carMock, begin());
    setup();
}

TEST_F(SmartcarManualControlFixture, receives_L_GoesLeft) {
    EXPECT_CALL(*serialMock, available())
        .WillOnce(Return(1));
    EXPECT_CALL(*serialMock, read())
        .WillOnce(Return('l'));
    EXPECT_CALL(*carMock, setSpeed(fSpeed));
    EXPECT_CALL(*carMock, setAngle(lDegrees));
    loop();
}


TEST_F(SmartcarManualControlFixture, receives_R_GoesRight) {
    EXPECT_CALL(*serialMock, available())
        .WillOnce(Return(1));
    EXPECT_CALL(*serialMock, read())
        .WillOnce(Return('r'));
    EXPECT_CALL(*carMock, setSpeed(fSpeed));
    EXPECT_CALL(*carMock, setAngle(rDegrees));
    loop();
}

TEST_F(SmartcarManualControlFixture, receives_F_GoesForward) {
    EXPECT_CALL(*serialMock, available())
        .WillOnce(Return(1));
    EXPECT_CALL(*serialMock, read())
        .WillOnce(Return('f'));
    EXPECT_CALL(*carMock, setSpeed(fSpeed));
    EXPECT_CALL(*carMock, setAngle(0));
    loop();
}

TEST_F(SmartcarManualControlFixture, receives_B_GoesForward) {
    EXPECT_CALL(*serialMock, available())
        .WillOnce(Return(1));
    EXPECT_CALL(*serialMock, read())
        .WillOnce(Return('b'));
    EXPECT_CALL(*carMock, setSpeed(bSpeed));
    EXPECT_CALL(*carMock, setAngle(0));
    loop();
}

TEST_F(SmartcarManualControlFixture, receives_AnythingElse_Stops) {
    EXPECT_CALL(*serialMock, available())
        .WillOnce(Return(1));
    EXPECT_CALL(*serialMock, read())
        .WillOnce(Return('s'));
    EXPECT_CALL(*carMock, setSpeed(0));
    EXPECT_CALL(*carMock, setAngle(0));
    loop();
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
