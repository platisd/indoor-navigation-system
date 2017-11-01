#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "arduino-mock/Arduino.h"

#include "../../src/wifi_module/wifi_module.ino"

using ::testing::Return;

class WifiModuleFixture : public ::testing::Test
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


TEST_F(WifiModuleFixture, wifiModuleSetup_whenSetupIsCalled_willSetPins)
{
    EXPECT_CALL(*arduinoMock, pinMode(WIFI_LED, OUTPUT));
    setup();
}

int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
