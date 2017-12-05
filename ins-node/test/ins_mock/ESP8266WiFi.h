#ifndef ESP8266WIFI_H
#define ESP8266WIFI_H

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <string>

// Some stubbed variables and types
const uint8_t WIFI_STA = 0;
const uint8_t WL_CONNECT_FAILED = 0;
const uint8_t WL_CONNECTED = 1;
using String = std::string; // Create an alias for Arduino String

// ESP8266 Mock class
class ESP8266Mock {
public:
    MOCK_METHOD1(BSSIDstr, String(int8_t));
    MOCK_METHOD1(RSSI, int32_t(int8_t));
    MOCK_METHOD0(scanNetworks, int8_t());
    MOCK_METHOD1(SSID, String(int8_t));
    MOCK_METHOD1(mode, void(uint8_t));
    MOCK_METHOD0(disconnect, void());
    MOCK_METHOD2(begin, void(const char[], const char[]));
    MOCK_METHOD0(status, uint8_t());
    MOCK_METHOD1(deepSleep, void(unsigned long));
    MOCK_METHOD2(connect, bool(const char[], uint16_t));
    MOCK_METHOD1(print, void(String));
    MOCK_METHOD0(available, int());
    MOCK_METHOD0(stop, void());
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

// Classes that inject our mock in the production code
class ESP8266WiFiClass {
public:
    String BSSIDstr(int8_t);
    int32_t RSSI(int8_t);
    int8_t scanNetworks();
    String SSID(int8_t);
    void mode(uint8_t);
    void disconnect();
    void begin(const char[], const char[]);
    uint8_t status();
};

class EspClass {
public:
    void deepSleep(unsigned long);
};

class WiFiClient {
public:
    bool connect(const char[], uint16_t);
    void print(String);
    int available();
    void stop();
};

// "Implementation" of classes that inject our mock
String ESP8266WiFiClass::BSSIDstr(int8_t index) {
    return esp8266Mock->BSSIDstr(index);
}

int32_t ESP8266WiFiClass::RSSI(int8_t index) {
    return esp8266Mock->RSSI(index);
}

int8_t ESP8266WiFiClass::scanNetworks() {
    return esp8266Mock->scanNetworks();
}

String ESP8266WiFiClass::SSID(int8_t index) {
    return esp8266Mock->SSID(index);
}

void ESP8266WiFiClass::mode(uint8_t wifiMode) {
    return esp8266Mock->mode(wifiMode);
}

void ESP8266WiFiClass::disconnect() {
    return esp8266Mock->disconnect();
}

void ESP8266WiFiClass::begin(const char ssid[], const char pass[]) {
    return esp8266Mock->begin(ssid, pass);
}

uint8_t ESP8266WiFiClass::status() {
    return esp8266Mock->status();
}

void EspClass::deepSleep(unsigned long time) {
    return esp8266Mock->deepSleep(time);
}

bool WiFiClient::connect(const char ip[], uint16_t port) {
    return esp8266Mock->connect(ip, port);
}

void WiFiClient::print(String s) {
    return esp8266Mock->print(s);
}

int WiFiClient::available() {
    return esp8266Mock->available();
}

void WiFiClient::stop() {
    return esp8266Mock->stop();
}

#endif
