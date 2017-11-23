#ifndef ESP8266WIFI_H
#define ESP8266WIFI_H

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <string>

// Some stubbed variables and types
const uint8_t WIFI_STA = 0;
const uint8_t WL_CONNECTED = 1;
using String = std::string; // Create an alias for Arduino String

// ESP8266WiFi Mock class
class ESP8266Mock {
public:
    ESP8266Mock();
    virtual ~ESP8266Mock() = default;
    MOCK_METHOD1(BSSIDstr, String(int8_t));
    MOCK_METHOD1(RSSI, int32_t(int8_t));
    MOCK_METHOD0(scanNetworks, int8_t());
    MOCK_METHOD1(SSID, String(int8_t));
    MOCK_METHOD1(mode, void(uint8_t));
    MOCK_METHOD0(disconnect, void());
    MOCK_METHOD2(begin, void(const char[], const char[]));
    MOCK_METHOD0(status, uint8_t());
    MOCK_METHOD1(deepSleep, void(unsigned long));
};

// Methods for instantiating and deleting the mock
static ESP8266Mock* esp8266Mock = NULL;
ESP8266Mock* esp8266MockInstance() {
    if(!esp8266Mock) {
        esp8266Mock = new ESP8266Mock();
    }
    return esp8266Mock;
}

void releaseEsp8266Mock() {
    if(esp8266Mock) {
        delete esp8266Mock;
        esp8266Mock = NULL;
    }
}

// Class that injects our mock in the production code
class ESP8266WiFi {
public:
    String BSSIDstr(int8_t);
    int32_t RSSI(int8_t);
    int8_t scanNetworks();
    String SSID(int8_t);
    void mode(uint8_t);
    void disconnect();
    void begin(const char[], const char[]);
    uint8_t status();
    void deepSleep(unsigned long);
};

extern ESP8266WiFi WiFi; // Variables used by the sketch
extern ESP8266WiFi ESP;

String ESP8266WiFi::BSSIDstr(int8_t index) {
    return esp8266Mock->BSSIDstr(index);
}

int32_t ESP8266WiFi::RSSI(int8_t index) {
    return esp8266Mock->RSSI(index);
}

int8_t ESP8266WiFi::scanNetworks() {
    return esp8266Mock->scanNetworks();
}

String ESP8266WiFi::SSID(int8_t index) {
    return esp8266Mock->SSID(index);
}

void ESP8266WiFi::mode(uint8_t wifiMode) {
    return esp8266Mock->mode(wifiMode);
}

void ESP8266WiFi::disconnect() {
    return esp8266Mock->disconnect();
}

void ESP8266WiFi::begin(const char ssid[], const char pass[]) {
    return esp8266Mock->begin(ssid, pass);
}

uint8_t ESP8266WiFi::status() {
    return esp8266Mock->status();
}

void ESP8266WiFi::deepSleep(unsigned long time) {
    return esp8266Mock->deepSleep(time);
}

#endif
