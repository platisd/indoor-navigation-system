#ifndef STEERING_MOTOR_H
#define STEERING_MOTOR_H

class SteeringMotorMock {
public:
    MOCK_METHOD1(setAngle, void(int));
    MOCK_METHOD0(getMaxLeftAngle, int());
    MOCK_METHOD0(getMaxRightAngle, int());
};

/* Methods for instantiating and deleting the mock */
static SteeringMotorMock* steeringMotorMock = NULL;
SteeringMotorMock* steeringMotorMockInstance() {
  if(!steeringMotorMock) {
    steeringMotorMock = new SteeringMotorMock();
  }
  return steeringMotorMock;
}

void releaseSteeringMotorMock() {
  if(steeringMotorMock) {
    delete steeringMotorMock;
    steeringMotorMock = NULL;
  }
}

/* ------------------------------------------------ */
class SteeringMotor {
public:
    SteeringMotor(){};
    ~SteeringMotor(){};
    void setAngle(int degrees){
        steeringMotorMock->setAngle(degrees);
    };
    int getMaxRightAngle(){
        steeringMotorMock->getMaxRightAngle();
    };
    int getMaxLeftAngle(){
        steeringMotorMock->getMaxLeftAngle();
    };
};

#endif // STEERING_MOTOR_H
