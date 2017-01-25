#ifndef ODOMETER_H
#define ODOMETER_H

#include "gmock/gmock.h"

class OdometerMock {
public:
    MOCK_METHOD1(attach, int(unsigned short));
    MOCK_METHOD3(attach, int(unsigned short, unsigned short, bool));
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
        encoderAttached = isInterruptPin(pin);
        return odometerMock->attach(pin);
    };
    int attach(unsigned short pin, unsigned short dir, bool f){
        encoderAttached = isInterruptPin(pin);
        return odometerMock->attach(pin, dir, f);
        };
    void begin(){
        assert(encoderAttached); // Encoder not attached!
        odometerMock->begin();
    };
    unsigned long getDistance(){
        assert(encoderAttached); // Encoder not attached!
        return odometerMock->getDistance();
    };
    float getSpeed(){
        assert(encoderAttached); // Encoder not attached!
        return odometerMock->getSpeed();
    };
private:
    bool encoderAttached;
    bool isInterruptPin(unsigned short pin){
        // External interrupt pins for Arduino Mega
        const unsigned short intPins[] = {2, 3, 18, 19, 20, 21};
        const unsigned int extInterrupts = sizeof(intPins)/sizeof(intPins[0]);
        for (int i = 0; i < extInterrupts; i++){
            if (intPins[i] == pin) return true;
        }
        return false;
    };
};

#endif // ODOMETER_H
