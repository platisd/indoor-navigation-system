
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Smartcar.h" // The Smartcar library mocks

#include "../../src/automatedMovements.ino" // Our production code

using ::testing::_; // Needed to use the "anything matcher"

class SmartcarAutomatedMovementsFixture : public ::testing::Test
{
   public:
     CarMock* carMock;
     OdometerMock* odometerMock;
     GyroscopeMock* gyroscopeMock;
    // Run this before the tests
    virtual void SetUp()
    {
      carMock = carMockInstance();
      odometerMock = odometerMockInstance();
      gyroscopeMock = gyroscopeMockInstance();
    }
    // Run this after the tests
    virtual void TearDown()
    {
      releaseCarMock();
      releaseOdometerMock();
      releaseGyroscopeMock();
    }
};

TEST_F(SmartcarAutomatedMovementsFixture, initsAreCalled) {
    EXPECT_CALL(*carMock, begin());
    EXPECT_CALL(*gyroscopeMock, attach());
    EXPECT_CALL(*gyroscopeMock, begin(_));
    EXPECT_CALL(*odometerMock, attach(_))
        .Times(2);
    EXPECT_CALL(*odometerMock, begin())
        .Times(2);
    setup();
}

TEST_F(SmartcarAutomatedMovementsFixture, rotatesAndGoes) {
    EXPECT_CALL(*carMock, go(40));
    EXPECT_CALL(*carMock, rotate(90));
    loop();
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
