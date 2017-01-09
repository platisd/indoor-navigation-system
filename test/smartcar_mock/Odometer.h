#ifndef ODOMETER_H
#define ODOMETER_H

#include "gmock/gmock.h"

class OdometerMock {
public:
    MOCK_METHOD1(attach, void(unsigned short));
    MOCK_METHOD0(begin, void());
    MOCK_METHOD0(getDistance, unsigned long());
    MOCK_METHOD0(getSpeed, float());
};

/* Methods for instantiating and deleting the mock */
static OdometerMock* odometerMock = NULL;
OdometerMock* odometerMockInstance() {
    if(!odometerMock) {
        odometerMock = new OdometerMock();
    }
    return odometerMock;
}

void releaseOdometerMock() {
    if(odometerMock) {
        delete odometerMock;
        odometerMock = NULL;
    }
}

/* -------------------------------------------- */
class Odometer {
public:
    Odometer(unsigned int pulsesPerMeter = 100){
        encoderAttached = false;
    };
    int attach(unsigned short pin){
        encoderAttached = true;
        odometerMock->attach(pin);
    };
    void begin(){
        assert(encoderAttached); // Encoder not attached!
        odometerMock->begin();
    };
    unsigned long getDistance(){
        assert(encoderAttached); // Encoder not attached!
        odometerMock->getDistance();
    };
    float getSpeed(){
        assert(encoderAttached); // Encoder not attached!
        odometerMock->getSpeed();
    };
private:
    bool encoderAttached;
};

#endif // ODOMETER_H
