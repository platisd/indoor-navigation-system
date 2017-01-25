#ifndef HEADING_SENSORS_H
#define HEADING_SENSORS_H

#include "gmock/gmock.h"

class GyroscopeMock {
public:
    MOCK_METHOD0(attach, void());
    MOCK_METHOD1(begin, void(unsigned short));
    MOCK_METHOD0(update, void());
    MOCK_METHOD0(getAngularDisplacement, unsigned int());
    MOCK_METHOD1(calibrate, int(int));
};

/* Methods for instantiating and deleting the mock */
static GyroscopeMock* gyroscopeMock = NULL;
GyroscopeMock* gyroscopeMockInstance() {
    if(!gyroscopeMock) {
        gyroscopeMock = new GyroscopeMock();
    }
    return gyroscopeMock;
}

void releaseGyroscopeMock() {
    if(gyroscopeMock) {
        delete gyroscopeMock;
        gyroscopeMock = NULL;
    }
}

/* -------------------------------------------- */

class HeadingSensor {
public:
    HeadingSensor(){};
    void update();
    unsigned int getAngularDisplacement();
};

class Gyroscope : public HeadingSensor {
public:
    Gyroscope(int offset = 0){
        gyroscopeAttached = false;
    };
    void attach(){
        gyroscopeAttached = true;
        gyroscopeMock->attach();
    };
    void begin(unsigned short samplingPeriod = 0){
        assert(gyroscopeAttached); // Gyroscope not attached!
        gyroscopeMock->begin(samplingPeriod);
    };
    void update(){
        assert(gyroscopeAttached); // Gyroscope not attached!
        gyroscopeMock->update();
    };
    int calibrate(int measurements = 100){
        assert(gyroscopeAttached); // Gyroscope not attached!
        return gyroscopeMock->calibrate(measurements);
    };
    unsigned int getAngularDisplacement(){
        assert(gyroscopeAttached); // Gyroscope not attached!
        return gyroscopeMock->getAngularDisplacement();
    };
private:
    bool gyroscopeAttached;
};

#endif // HEADING_SENSORS_H
