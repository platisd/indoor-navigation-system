#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "arduino-mock/Arduino.h"
#include "ESP8266WiFi.h"

#include "../../src/wifi_module/wifi_module.ino"

using ::testing::Return;

class WifiModuleFixture : public ::testing::Test
{
   public:
    ArduinoMock* arduinoMock;
    ESP8266Mock* esp8266Mock;

    // Run this before the tests
    virtual void SetUp()
    {
        arduinoMock = arduinoMockInstance();
        esp8266Mock = esp8266MockInstance();
    }
    // Run this after the tests
    virtual void TearDown()
    {
        releaseArduinoMock();
        releaseEsp8266Mock();
    }
};


TEST_F(WifiModuleFixture, wifiModuleSetup_whenSetupIsCalled_willSetPins)
{
}

int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
