#ifndef CARMOCK_H
#define CARMOCK_H

#include "HeadingSensors.h"
#include "Odometer.h"
#include "SteeringMotor.h"
#include "ThrottleMotor.h"

class CarMock {
public:
    MOCK_METHOD0(begin, void());
    MOCK_METHOD1(setSpeed, void(float));
    MOCK_METHOD1(setAngle, void(int));
    MOCK_METHOD0(getSpeed, float());
    MOCK_METHOD0(getAngle, int());
    MOCK_METHOD0(stop, void());
    MOCK_METHOD4(enableCruiseControl, void(float, float, float, unsigned short));
    MOCK_METHOD0(disableCruiseControl, void());
    MOCK_METHOD0(updateMotors, void());
    MOCK_METHOD1(go, void(int));
    MOCK_METHOD1(rotate, void(int));
    MOCK_METHOD2(setMotorSpeed, void(int, int));
    MOCK_METHOD0(cruiseControlEnabled, bool());
};

/* Methods for instantiating and deleting the mock */
static CarMock* carMock = NULL;
CarMock* carMockInstance() {
    if(!carMock) {
        carMock = new CarMock();
    }
    return carMock;
}

void releaseCarMock() {
    if(carMock) {
        delete carMock;
        carMock = NULL;
    }
}
/* ------------------------------------------------ */

class Car {
public:
    Car(unsigned short shieldOrientation = 0){
        encoderAttached = false;
        headingAttached = false;
        carHasBegun = false;
    }
    Car(SteeringMotor *steering, ThrottleMotor *throttle){
        encoderAttached = false;
        headingAttached = false;
        carHasBegun = false;
    };
    void begin(){
        carHasBegun = true;
        carMock->begin();
    };
    void begin(Odometer &encoder){
        carHasBegun = true;
        encoderAttached = true;
        headingAttached = false;
        carMock->begin();
    };
    void begin(HeadingSensor &heading){
        carHasBegun = true;
        encoderAttached = false;
        headingAttached = true;
        carMock->begin();
    };
    void begin(Odometer &encoder, HeadingSensor &heading){
        carHasBegun = true;
        encoderAttached = true;
        headingAttached = true;
        carMock->begin();
    };
    void begin(Odometer &encoder1, Odometer &encoder2){
        carHasBegun = true;
        encoderAttached = true;
        headingAttached = false;
        carMock->begin();
    };
    void begin(Odometer &encoder1, Odometer &encoder2, HeadingSensor &heading){
        carHasBegun = true;
        encoderAttached = true;
        headingAttached = true;
        carMock->begin();
    };
    void setSpeed(float speed){
        assert(carHasBegun); // Car has not begun!
        carMock->setSpeed(speed);
    };
    void setAngle(int degrees){
        assert(carHasBegun); // Car has not begun!
        carMock->setAngle(degrees);
    };
    float getSpeed(){
        assert(carHasBegun); // Car has not begun!
        return carMock->getSpeed();
    };
    int getAngle(){
        assert(carHasBegun); // Car has not begun!
        return carMock->getAngle();
    };
    void stop(){
        assert(carHasBegun); // Car has not begun!
        carMock->stop();
    };
    void enableCruiseControl(float Kp = 0, float Ki = 0, float Kd = 0, unsigned short frequency = 80){
        assert(carHasBegun); // Car has not begun!
        assert(encoderAttached); // No odometer attached!
        carMock->enableCruiseControl(Kp, Ki, Kd, frequency);
    };
    void disableCruiseControl(){
        assert(carHasBegun); // Car has not begun!
        carMock->disableCruiseControl();
    };
    void updateMotors(){
        assert(carHasBegun); // Car has not begun!
        assert(encoderAttached); // No odometer attached!
        carMock->updateMotors();
    };
    void go(int centimeters){
        assert(carHasBegun); // Car has not begun!
        assert(encoderAttached); // No odometer attached!
        carMock->go(centimeters);
    };
    void rotate(int degrees){
        assert(carHasBegun); // Car has not begun!
        assert(headingAttached); // No heading sensor attached!
        carMock->rotate(degrees);
    };
    void setMotorSpeed(int leftMotorSpeed, int rightMotorSpeed){
        assert(carHasBegun); // Car has not begun!
        carMock->setMotorSpeed(leftMotorSpeed, rightMotorSpeed);
    };
    bool cruiseControlEnabled(){
        assert(carHasBegun); // Car has not begun!
        return carMock->cruiseControlEnabled();
    };
private:
    bool encoderAttached, headingAttached, carHasBegun;
};

#endif // CARMOCK_H
