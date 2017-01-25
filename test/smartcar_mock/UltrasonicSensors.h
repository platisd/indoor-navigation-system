#ifndef ULTRASONIC_SENSORS_H
#define ULTRASONIC_SENSORS_H

#include "gmock/gmock.h"

class SR04Mock {
public:
    MOCK_METHOD2(attach, void(unsigned short, unsigned short));
    MOCK_METHOD0(getDistance, unsigned int());
    MOCK_METHOD1(getMedianDistance, unsigned int(short));
};

/* Methods for instantiating and deleting the mock */
static SR04Mock* SR04_mock = NULL;
SR04Mock* SR04MockInstance() {
    if(!SR04_mock) {
        SR04_mock = new SR04Mock();
    }
    return SR04_mock;
}

void releaseSR04Mock() {
    if(SR04_mock) {
        delete SR04_mock;
        SR04_mock = NULL;
    }
}

class SRF08Mock {
public:
    MOCK_METHOD1(attach, void(unsigned short));
    MOCK_METHOD0(getDistance, unsigned int());
    MOCK_METHOD1(getMedianDistance, unsigned int(short));
    MOCK_METHOD1(setGain, void(unsigned short));
    MOCK_METHOD1(setRange, void(unsigned short));
    MOCK_METHOD1(setPingDelay, void(unsigned short));
    MOCK_METHOD1(changeAddress, void(unsigned short));
    MOCK_METHOD0(getLightReading, unsigned short());
};

/* Methods for instantiating and deleting the mock */
static SRF08Mock* SRF08_mock = NULL;
SRF08Mock* SRF08MockInstance() {
    if(!SRF08_mock) {
        SRF08_mock = new SRF08Mock();
    }
    return SRF08_mock;
}

void releaseSRF08Mock() {
    if(SRF08_mock) {
        delete SRF08_mock;
        SRF08_mock = NULL;
    }
}

/* -------------------------------------------- */
class SR04 {
public:
    SR04(unsigned int maxDistance = 100){
        SR04attached = false;
    };
    void attach(unsigned short triggerPin, unsigned short echoPin){
        SR04attached = true;
        SR04_mock->attach(triggerPin, echoPin);
    };
    unsigned int getDistance(){
        assert(SR04attached); // SR04 not attached!
        return SR04_mock->getDistance();
    };
    unsigned int getMedianDistance(short iterations = 5){
        assert(SR04attached); // SR04 not attached!
        return SR04_mock->getMedianDistance(iterations);
    };
private:
    bool SR04attached;
};

class SRF08 {
public:
    SRF08(){
        SRF08attached = false;
    }
    void attach(unsigned short address = 112){
        // Verify first that address is in the correct range
        assert((address >= 112) && (address <= 127)); // Wrong address!
        SRF08attached = true;
        SRF08_mock->attach(address);
    };
    unsigned int getDistance(){
        assert(SRF08attached); // SRF08 not attached!
        return SRF08_mock->getDistance();
    };
    unsigned int getMedianDistance(short iterations = 5){
        assert(SRF08attached); // SRF08 not attached!
        return SRF08_mock->getMedianDistance(iterations);
    };
    void setGain(unsigned short gainValue){
        assert(SRF08attached); // SRF08 not attached!
        SRF08_mock->setGain(gainValue);
    };
    void setRange(unsigned short rangeValue){
        assert(SRF08attached); // SRF08 not attached!
        SRF08_mock->setRange(rangeValue);
    };
    void setPingDelay(unsigned short milliseconds = 70){
        assert(SRF08attached); // SRF08 not attached!
        SRF08_mock->setPingDelay(milliseconds);
    };
    void changeAddress(unsigned short newAddress){
        assert(SRF08attached); // SRF08 not attached!
        SRF08_mock->changeAddress(newAddress);
    };
    unsigned short getLightReading(){
        assert(SRF08attached); // SRF08 not attached!
        return SRF08_mock->getLightReading();
    };
private:
    bool SRF08attached;
};

#endif // ULTRASONIC_SENSORS_H
