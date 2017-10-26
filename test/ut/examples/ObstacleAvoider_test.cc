#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Smartcar.h" // The Smartcar library mocks

#include "../../src/obstacleAvoider.ino" // Our production code

using ::testing::_; // Needed to use the "anything matcher"
using ::testing::Return;

class SmartcarObstacleAvoiderFixture : public ::testing::Test
{
   public:
     CarMock* carMock;
     GyroscopeMock* gyroscopeMock;
     SR04Mock* SR04_mock;
    // Run this before the tests
    virtual void SetUp()
    {
      carMock = carMockInstance();
      gyroscopeMock = gyroscopeMockInstance();
      SR04_mock = SR04MockInstance();
    }
    // Run this after the tests
    virtual void TearDown()
    {
      releaseCarMock();
      releaseGyroscopeMock();
      releaseSR04Mock();
    }
};

TEST_F(SmartcarObstacleAvoiderFixture, initsAreCalled) {
    EXPECT_CALL(*carMock, begin());
    EXPECT_CALL(*gyroscopeMock, attach());
    EXPECT_CALL(*gyroscopeMock, begin(_));
    EXPECT_CALL(*SR04_mock, attach(_, _));
    setup();
}

TEST_F(SmartcarObstacleAvoiderFixture, givenObstacleFound_rotates) {
    EXPECT_CALL(*SR04_mock, getDistance())
    .WillOnce(Return(5));
    EXPECT_CALL(*carMock, setSpeed(100));
    EXPECT_CALL(*carMock, rotate(90));
    loop();
}

TEST_F(SmartcarObstacleAvoiderFixture, noObstacleFound_slowsDown) {
    EXPECT_CALL(*SR04_mock, getDistance())
    .WillOnce(Return(70));
    EXPECT_CALL(*carMock, setSpeed(50));
    loop();
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
