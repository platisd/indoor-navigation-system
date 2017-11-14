#ifndef LOW_POWER_MOCK_H
#define LOW_POWER_MOCK_H

#include "gtest/gtest.h"
#include "gmock/gmock.h"

// Mocks for free functions from avr/power.h, avr/sleep.h and avr/wdt.h
class LowPowerMock {
public:
    MOCK_METHOD0(wdt_disable, void());
    MOCK_METHOD1(set_sleep_mode, void(unsigned int));
    MOCK_METHOD0(power_all_disable, void());
    MOCK_METHOD0(power_all_enable, void());
    MOCK_METHOD0(sleep_cpu, void());
    MOCK_METHOD0(sleep_disable, void());
    MOCK_METHOD0(sleep_enable, void());
    MOCK_METHOD0(wdt_reset, void());
};

// Methods for instantiating and deleting the mock
static LowPowerMock* lowPowerMock = NULL;
LowPowerMock* lowPowerMockInstance() {
    if(!lowPowerMock) {
        lowPowerMock = new LowPowerMock();
    }
    return lowPowerMock;
}

void releaseLowPowerMock() {
    if(lowPowerMock) {
        delete lowPowerMock;
        lowPowerMock = NULL;
    }
}

// Get the free standing functions to return mocked methods

void wdt_disable() {
    lowPowerMock->wdt_disable();
}

void set_sleep_mode(unsigned int mode) {
    lowPowerMock->set_sleep_mode(mode);
}

void power_all_disable() {
    lowPowerMock->power_all_disable();
}

void power_all_enable() {
    lowPowerMock->power_all_enable();
}

void sleep_cpu() {
    lowPowerMock->sleep_cpu();
}

void sleep_disable() {
    lowPowerMock->sleep_disable();
}

void sleep_enable() {
    lowPowerMock->sleep_enable();
}

void wdt_reset() {
    lowPowerMock->wdt_reset();
}

#endif
