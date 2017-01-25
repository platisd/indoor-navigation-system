#ifndef INFRARED_SENSORS_H
#define INFRARED_SENSORS_H

#include "gmock/gmock.h"

/* GP2D120 SHARP infrared sensor (5-25cm) */
class GP2D120Mock {
public:
    MOCK_METHOD1(attach, void(unsigned short pin));
    MOCK_METHOD0(getDistance, unsigned int());
    MOCK_METHOD1(getMedianDistance, unsigned int(short iterations));
};

/* Methods for instantiating and deleting the mock */
static GP2D120Mock* GP2D120_mock = NULL;
GP2D120Mock* GP2D120MockInstance() {
    if(!GP2D120_mock) {
        GP2D120_mock = new GP2D120Mock();
    }
    return GP2D120_mock;
}

void releaseGP2D120Mock() {
    if(GP2D120_mock) {
        delete GP2D120_mock;
        GP2D120_mock = NULL;
    }
}

/* GP2Y0A02 SHARP infrared sensor (25-120cm) */
class GP2Y0A02Mock {
public:
    MOCK_METHOD1(attach, void(unsigned short pin));
    MOCK_METHOD0(getDistance, unsigned int());
    MOCK_METHOD1(getMedianDistance, unsigned int(short iterations));
};

/* Methods for instantiating and deleting the mock */
static GP2Y0A02Mock* GP2Y0A02_mock = NULL;
GP2Y0A02Mock* GP2Y0A02MockInstance() {
    if(!GP2Y0A02_mock) {
        GP2Y0A02_mock = new GP2Y0A02Mock();
    }
    return GP2Y0A02_mock;
}

void releaseGP2Y0A02Mock() {
    if(GP2Y0A02_mock) {
        delete GP2Y0A02_mock;
        GP2Y0A02_mock = NULL;
    }
}

/* GP2Y0A21 SHARP infrared sensor (12-78cm) */
class GP2Y0A21Mock {
public:
    MOCK_METHOD1(attach, void(unsigned short pin));
    MOCK_METHOD0(getDistance, unsigned int());
    MOCK_METHOD1(getMedianDistance, unsigned int(short iterations));
};

/* Methods for instantiating and deleting the mock */
static GP2Y0A21Mock* GP2Y0A21_mock = NULL;
GP2Y0A21Mock* GP2Y0A21MockInstance() {
    if(!GP2Y0A21_mock) {
        GP2Y0A21_mock = new GP2Y0A21Mock();
    }
    return GP2Y0A21_mock;
}

void releaseGP2Y0A21Mock() {
    if(GP2Y0A21_mock) {
        delete GP2Y0A21_mock;
        GP2Y0A21_mock = NULL;
    }
}

/* -------------------------------------------- */
class GP2D120 {
public:
    GP2D120(){
        GP2D120attached = false;
    };
    void attach(unsigned short pin){
        GP2D120attached = true;
        GP2D120_mock->attach(pin);
    };
    unsigned int getDistance(){
        assert(GP2D120attached); // GP2D120 not attached!
        return GP2D120_mock->getDistance();
    };
    unsigned int getMedianDistance(short iterations = 5){
        assert(GP2D120attached); // GP2D120 not attached!
        return GP2D120_mock->getMedianDistance(iterations);
    };
private:
    bool GP2D120attached;
};

class GP2Y0A02 {
public:
    GP2Y0A02(){
        GP2Y0A02attached = false;
    };
    void attach(unsigned short pin){
        GP2Y0A02attached = true;
        GP2Y0A02_mock->attach(pin);
    };
    unsigned int getDistance(){
        assert(GP2Y0A02attached); // GP2Y0A02 not attached!
        return GP2Y0A02_mock->getDistance();
    };
    unsigned int getMedianDistance(short iterations){
        assert(GP2Y0A02attached); // GP2Y0A02 not attached!
        return GP2Y0A02_mock->getMedianDistance(iterations);
    };
private:
    bool GP2Y0A02attached;
};

class GP2Y0A21 {
public:
    GP2Y0A21(){
        GP2Y0A21attached = false;
    };
    void attach(unsigned short pin){
        GP2Y0A21attached = true;
        GP2Y0A21_mock->attach(pin);
    };
    unsigned int getDistance(){
        assert(GP2Y0A21attached); // GP2Y0A21 not attached!
        return GP2Y0A21_mock->getDistance();
    };
    unsigned int getMedianDistance(short iterations){
        assert(GP2Y0A21attached); // GP2Y0A21 not attached!
        return GP2Y0A21_mock->getMedianDistance(iterations);
    };
private:
    bool GP2Y0A21attached;
};

#endif // INFRARED_SENSORS_H
