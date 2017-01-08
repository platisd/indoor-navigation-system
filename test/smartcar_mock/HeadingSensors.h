#ifndef HEADING_SENSORS_H
#define HEADING_SENSORS_H

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
    Gyroscope(int offset = 0){};
    void attach(){
        gyroscopeMock->attach();
    };
    void begin(unsigned short samplingPeriod = 0){
        gyroscopeMock->begin(samplingPeriod);
    };
    void update(){
        gyroscopeMock->update();
    };
    int calibrate(int measurements = 100){
        gyroscopeMock->calibrate(measurements);
    };
    unsigned int getAngularDisplacement(){
        gyroscopeMock->getAngularDisplacement();
    };
};

#endif // HEADING_SENSORS_H
