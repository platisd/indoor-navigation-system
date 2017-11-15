#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "arduino-mock/Arduino.h"
#include "power_controller_utils.h"
#include "LowPowerMock.h"

#include "../../src/power_controller/power_controller.ino"

using ::testing::Return;
using ::testing::InSequence;
using ::testing::_;

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
          // Re-initialize the global variables
          timeWeHaveSlept = 0;
          watchDogBarked = false;
          watchDogEnabled = false;
          signalFromWifiModule = false;
          currentState = SLEEP_WIFI_OFF;
    }
protected:
    void expectDeepSleep()
    {
        // Expect deep sleep and wake up methods to always be invoked sequentially
        InSequence seq;
        EXPECT_CALL(*lowPowerMock, set_sleep_mode(SLEEP_MODE_PWR_DOWN));
        EXPECT_CALL(*lowPowerMock, power_all_disable());
        EXPECT_CALL(*lowPowerMock, sleep_enable());
        EXPECT_CALL(*lowPowerMock, sleep_cpu());
        EXPECT_CALL(*lowPowerMock, sleep_disable());
        EXPECT_CALL(*lowPowerMock, power_all_enable());
    }
};


TEST_F(PowerControllerFixture, setup_whenSetupIsCalled_willInitializePins)
{
    // Pins are initialized as inputs or outputs
    EXPECT_CALL(*arduinoMock, pinMode(RX_PIN, INPUT));
    EXPECT_CALL(*arduinoMock, pinMode(TX_PIN, OUTPUT));
    EXPECT_CALL(*arduinoMock, pinMode(WIFI_MODULE_PIN, OUTPUT));

    // Output pin initial states are set
    EXPECT_CALL(*arduinoMock, digitalWrite(TX_PIN, LOW));
    EXPECT_CALL(*arduinoMock, digitalWrite(WIFI_MODULE_PIN, LOW));

    setup();
}

TEST_F(PowerControllerFixture, stayAsleep_whenCalled_willSleep)
{
    const unsigned long sleepDuration = 1000;

    expectDeepSleep();

    stayAsleepFor(sleepDuration);
}

TEST_F(PowerControllerFixture, stayAsleep_whenWatchdogIsEnabled_willNotReenableIt)
{
    const unsigned long sleepDuration = WIFI_OFF_SLEEP_DURATION;
    watchDogEnabled = true;

    // Expect watchdog functions NOT to be called
    EXPECT_CALL(*lowPowerMock, wdt_reset()).Times(0);
    expectDeepSleep();

    stayAsleepFor(sleepDuration);
}

TEST_F(PowerControllerFixture, stayAsleep_whenWatchdogIsDisabled_willEnableIt)
{
    const unsigned long sleepDuration = WIFI_OFF_SLEEP_DURATION;
    watchDogEnabled = false; // It is already false, just making it obvious

    // Expect watchdog functions to be called
    EXPECT_CALL(*lowPowerMock, wdt_reset());
    expectDeepSleep();

    stayAsleepFor(sleepDuration);
}

TEST_F(PowerControllerFixture, stayAsleep_whenTimeHasNotPassed_willReturnFalse)
{
    const unsigned long sleepDuration = WIFI_OFF_SLEEP_DURATION;
    timeWeHaveSlept = 0; // It is already 0, just making it obvious
    watchDogBarked = false; // The watchdog should have barked so to check time

    expectDeepSleep();

    ASSERT_FALSE(stayAsleepFor(sleepDuration));
}

TEST_F(PowerControllerFixture, stayAsleep_whenWatchDogDidNotBark_willReturnFalse)
{
    const unsigned long sleepDuration = WIFI_OFF_SLEEP_DURATION;
    watchDogBarked = false; // The watchdog should have barked so to check time

    expectDeepSleep();

    ASSERT_FALSE(stayAsleepFor(sleepDuration));
}

TEST_F(PowerControllerFixture, stayAsleep_whenTimeHasPassed_willReturnTrue)
{
    const unsigned long sleepDuration = WIFI_OFF_SLEEP_DURATION;
    timeWeHaveSlept = sleepDuration; // We have slept as much as we should
    watchDogBarked = true; // The watchdog should have barked so to check time

    expectDeepSleep();

    ASSERT_TRUE(stayAsleepFor(sleepDuration));
}

TEST_F(PowerControllerFixture, loop_whenWifiOffAndNotTimeToWake_willRemainInSleepWifiOff)
{
    timeWeHaveSlept = 0;
    currentState = SLEEP_WIFI_OFF;
    watchDogBarked = true;

    // Expect the WiFi NOT to be turned on
    EXPECT_CALL(*arduinoMock, digitalWrite(WIFI_MODULE_PIN, HIGH)).Times(0);
    expectDeepSleep();

    loop();

    // The state should have remained the same
    ASSERT_EQ(currentState, SLEEP_WIFI_OFF);
}

TEST_F(PowerControllerFixture, loop_whenWifiOffAndTimeToWake_willTurnWifiOn)
{
    timeWeHaveSlept = WIFI_OFF_SLEEP_DURATION;
    currentState = SLEEP_WIFI_OFF;
    watchDogBarked = true; // To check the new time

    // Expect the Wifi to be turned on
    EXPECT_CALL(*arduinoMock, digitalWrite(WIFI_MODULE_PIN, HIGH));
    expectDeepSleep();

    loop();

    // The state should have changed
    ASSERT_EQ(currentState, SLEEP_WIFI_ON);
}

TEST_F(PowerControllerFixture, loop_whenWifiOnAndNotTimeToSleep_willRemainWifiOn)
{
    timeWeHaveSlept = 0;
    currentState = SLEEP_WIFI_ON;
    watchDogBarked = true; // To check the new time

    // Expect NO pulse to be sent
    EXPECT_CALL(*arduinoMock, digitalWrite(TX_PIN, _)).Times(0);
    expectDeepSleep();

    loop();

    // The state should have remained
    ASSERT_EQ(currentState, SLEEP_WIFI_ON);
}

TEST_F(PowerControllerFixture, loop_whenWifiOnAndTimeToSleep_willNotifyShutdown)
{
    timeWeHaveSlept = WIFI_ON_SLEEP_DURATION; // Have slept as much as we should
    currentState = SLEEP_WIFI_ON;
    watchDogBarked = true; // To check the new time

    { // Expect pulse to be sent
        InSequence seq;
        EXPECT_CALL(*arduinoMock, digitalWrite(TX_PIN, HIGH));
        EXPECT_CALL(*arduinoMock, digitalWrite(TX_PIN, LOW));
    }
    expectDeepSleep();

    loop();

    // The state should have changed
    ASSERT_EQ(currentState, PREPARE_SHUTDOWN);
}

TEST_F(PowerControllerFixture, loop_whenWifiOnAndSignalArrives_willNotifyShutdown)
{
    timeWeHaveSlept = 0; // Have NOT slept as much as we should
    currentState = SLEEP_WIFI_ON;
    watchDogBarked = false; // The watchdog HAS NOT barked
    signalFromWifiModule = true; // Signal from wifi has arrived

    { // Expect pulse to be sent
        InSequence seq;
        EXPECT_CALL(*arduinoMock, digitalWrite(TX_PIN, HIGH));
        EXPECT_CALL(*arduinoMock, digitalWrite(TX_PIN, LOW));
    }
    expectDeepSleep();

    loop();

    // The state should have changed
    ASSERT_EQ(currentState, PREPARE_SHUTDOWN);
}

TEST_F(PowerControllerFixture, loop_whenPreparingShutdownNotElapsed_willNotTurnWifiOff)
{
    timeWeHaveSlept = 0;
    currentState = PREPARE_SHUTDOWN;
    watchDogBarked = true;

    // Expect wifi to be turned off
    EXPECT_CALL(*arduinoMock, digitalWrite(WIFI_MODULE_PIN, LOW)).Times(0);
    expectDeepSleep();

    loop();

    // The state should have remained
    ASSERT_EQ(currentState, PREPARE_SHUTDOWN);
}

TEST_F(PowerControllerFixture, loop_whenPreparingShutdownElapsed_willTurnWifiOff)
{
    timeWeHaveSlept = WIFI_PREPARE_SHUTDOWN_DURATION; // Have slept as much as we should
    currentState = PREPARE_SHUTDOWN;
    watchDogBarked = true;

    // Expect wifi to be turned off
    EXPECT_CALL(*arduinoMock, digitalWrite(WIFI_MODULE_PIN, LOW));
    expectDeepSleep();

    loop();

    // The state should have changed
    ASSERT_EQ(currentState, SLEEP_WIFI_OFF);
}

TEST_F(PowerControllerFixture, loop_whenPreparingShutdownAndSignalArrives_willGoToWifiOn)
{
    timeWeHaveSlept = 0;
    currentState = PREPARE_SHUTDOWN;
    watchDogBarked = false; // Watchdog did not bark
    signalFromWifiModule = true; // Signal from wifi has arrived

    // Expect wifi NOT to be turned off
    EXPECT_CALL(*arduinoMock, digitalWrite(WIFI_MODULE_PIN, LOW)).Times(0);
    expectDeepSleep();

    loop();

    // The state should have changed
    ASSERT_EQ(currentState, SLEEP_WIFI_ON);
}

int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
