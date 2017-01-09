#ifndef THROTTLE_MOTOR_H
#define THROTTLE_MOTOR_H

#include "gmock/gmock.h"

class ThrottleMotorMock {
public:
    MOCK_METHOD1(setSpeed, void(int));
    MOCK_METHOD2(setMotorSpeed, void(int,int));
};

/* Methods for instantiating and deleting the mock */
static ThrottleMotorMock* throttleMotorMock = NULL;
ThrottleMotorMock* throttleMotorMockInstance() {
  if(!throttleMotorMock) {
    throttleMotorMock = new ThrottleMotorMock();
  }
  return throttleMotorMock;
}

void releaseThrottleMotorMock() {
  if(throttleMotorMock) {
    delete throttleMotorMock;
    throttleMotorMock = NULL;
  }
}

/* ------------------------------------------------ */
class ThrottleMotor {
public:
    ThrottleMotor(){};
    ~ThrottleMotor(){};
    void setSpeed(int speed){
        throttleMotorMock->setSpeed(speed);
    };
    void setMotorSpeed(int leftMotorSpeed, int rightMotorSpeed){
        throttleMotorMock->setMotorSpeed(leftMotorSpeed, rightMotorSpeed);
    };
};

#endif // THROTTLE_MOTOR_H
