#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "arduino-mock/Arduino.h"
#include "test_utils.h"

#include "../../src/ins_node.ino"

using ::testing::Return;

class InsNodeFixture : public ::testing::Test
{
   public:
     ArduinoMock* arduinoMock;

    // Run this before the tests
    virtual void SetUp()
    {
      arduinoMock = arduinoMockInstance();
    }
    // Run this after the tests
    virtual void TearDown()
    {
      releaseArduinoMock();
    }
};


TEST_F(InsNodeFixture, insNodeSetup_whenWifiIsTurnedOn_willNotCrash)
{
  uint8_t expected_error = 0;
  EXPECT_CALL(*arduinoMock, digitalWrite(WIFI_MODULE_PIN, HIGH));
  ASSERT_EQ(turnWifi(HIGH), expected_error);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
