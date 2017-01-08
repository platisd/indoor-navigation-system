#ifndef ODOMETER_H
#define ODOMETER_H

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
    Odometer(unsigned int pulsesPerMeter = 100){};
    int attach(unsigned short pin){
        odometerMock->attach(pin);
    };
    void begin(){
        odometerMock->begin();
    };
    unsigned long getDistance(){
        odometerMock->getDistance();
    };
    float getSpeed(){
        odometerMock->getSpeed();
    };
};

#endif // ODOMETER_H
