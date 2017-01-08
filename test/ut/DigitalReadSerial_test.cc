// Copyright 2014 http://switchdevice.com
// This example code is in the public domain.

#include "gtest/gtest.h"
#include "arduino-mock/Arduino.h"
#include "arduino-mock/Serial.h"

#include "../../src/DigitalReadSerial.ino"

using ::testing::Return;

class DigitalReadTestFixture : public ::testing::Test
{
   public:
     ArduinoMock* arduinoMock;
     SerialMock* serialMock;
    // Run this before the tests
    virtual void SetUp()
    {
      arduinoMock = arduinoMockInstance();
      serialMock = serialMockInstance();
    }
    // Run this after the tests
    virtual void TearDown()
    {
      releaseSerialMock();
      releaseArduinoMock();
    }
};


TEST_F(DigitalReadTestFixture, pushed) {
  EXPECT_CALL(*arduinoMock, digitalRead(2))
    .WillOnce(Return(1));
  EXPECT_CALL(*serialMock, println(1, 10));
  EXPECT_CALL(*arduinoMock, delay(1));
  loop();
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
