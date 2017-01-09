#ifndef HELPER_METHODS_H
#define HELPER_METHODS_H

#include "gmock/gmock.h"
#include "ThrottleMotor.h"
#include "SteeringMotor.h"

class DCSteerThrottle : public ThrottleMotor, public SteeringMotor {
public:
    DCSteerThrottle(){};
    ~DCSteerThrottle(){};
};

class ServoMotor : public SteeringMotor {
public:
    ServoMotor(){};
    ~ServoMotor(){};
};

class ESCMotor : public ThrottleMotor {
public:
    ESCMotor(){};
    ~ESCMotor(){};
};

class ShieldMotors : public ThrottleMotor, public SteeringMotor {
public:
    ShieldMotors(){};
    ~ShieldMotors(){};
};

/* Some fakes to make everything compile. TO-DO: Make mocks out of them */
DCSteerThrottle* useDCMotor(short shieldSide) {
    return new DCSteerThrottle();
}

DCSteerThrottle* useDCMotor(unsigned short directionPinA, unsigned short directionPinB, unsigned short enablePin){
    return new DCSteerThrottle();
}

ServoMotor* useServo(unsigned short servoPin){
    return new ServoMotor();
}

ESCMotor* useESC(unsigned short escPin){
    return new ESCMotor();
}

ShieldMotors* useShieldMotors(unsigned short shieldOrientation = 0){
    return new ShieldMotors();
}

#endif // HELPER_METHODS_H
