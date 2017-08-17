#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "arduino-mock/Arduino.h"
#include "arduino-mock/Serial.h"
#include "Smartcar.h" // The Smartcar library mocks

#include "../../src/MultipleSR04.ino" // Our production code


using ::testing::InSequence;
using ::testing::Return;
using ::testing::_;

class SR04Fixture : public ::testing::Test
{
public:
    ArduinoMock* arduinoMock; // Necessary for delay()
    SR04Mock* SR04_mock;
    // Run this before the tests
    virtual void SetUp()
    {
        arduinoMock = arduinoMockInstance();
        SR04_mock = SR04MockInstance();
    }
    // Run this after the tests
    virtual void TearDown()
    {
        releaseArduinoMock();
        releaseSR04Mock();
    }
};

TEST_F(SR04Fixture, initsAreCalled) {
    {
        InSequence seq;
        // Everything in this scope has to happen in the specified sequence
        EXPECT_CALL(*SR04_mock, attach(3, 4));
        EXPECT_CALL(*SR04_mock, attach(6, 7));
    }
    setup();
}

TEST_F(SR04Fixture, expectGetDistanceCall) {
    // If you want to specify a sequence of different expects use InSequence
    // {
    //     InSequence seq;
    //     EXPECT_CALL(*SR04_mock, getDistance())
    //         .WillOnce(Return(15));
    //     EXPECT_CALL(*SR04_mock, getDistance())
    //         .WillOnce(Return(25));
    // }
    // Alternatively, if you expect sequential calls of a specific instance
    EXPECT_CALL(*SR04_mock, getDistance())
        .Times(2)
        .WillOnce(Return(15))
        .WillOnce(Return(25));

    loop();
    // After we have run loop, we can test whether d1 and d2 got the values
    ASSERT_EQ(d1, 15);
    ASSERT_EQ(d2, 25);
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
