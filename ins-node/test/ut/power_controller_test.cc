#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "arduino-mock/Arduino.h"
#include "power_controller_utils.h"
#include "LowPowerMock.h"

#include "../../src/power_controller/power_controller.ino"

using ::testing::Return;

class PowerControllerFixture : public ::testing::Test
{
   public:
    ArduinoMock* arduinoMock;
    LowPowerMock* lowPowerMock;

    // Run this before the tests
    virtual void SetUp()
    {
          arduinoMock = arduinoMockInstance();
          lowPowerMock = lowPowerMockInstance();
    }
    // Run this after the tests
    virtual void TearDown()
    {
          releaseArduinoMock();
          releaseLowPowerMock();
    }
};


TEST_F(PowerControllerFixture, powerControllerSetup_whenWifiIsTurnedOn_willNotCrash)
{
}

int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
