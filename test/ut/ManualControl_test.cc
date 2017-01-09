
#include "gtest/gtest.h"
#include "arduino-mock/Arduino.h" // Necessary to include for Serial
#include "arduino-mock/Serial.h"
#include "Smartcar.h" // The Smartcar library mocks

#include "../../src/manualControl.ino" // Our production code

using ::testing::Return; // Needed to use "Return"
using ::testing::_; // Needed to use the "anything matcher"

class SmartcarManualControlFixture : public ::testing::Test
{
   public:
     SerialMock* serialMock;
     CarMock* carMock;
    // Run this before the tests
    virtual void SetUp()
    {
      serialMock = serialMockInstance();
      carMock = carMockInstance();
    }
    // Run this after the tests
    virtual void TearDown()
    {
      releaseSerialMock();
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
