#include <Smartcar.h>

Odometer encoderLeft, encoderRight;
Gyroscope gyro;
Car car(useServo(4), useDCMotor(8,7,6));

void setup() {
  gyro.attach();
  encoderLeft.attach(2);
  encoderRight.attach(3);
  encoderLeft.begin();
  encoderRight.begin();
  car.begin(encoderLeft, encoderRight,gyro);
  car.enableCruiseControl();
  gyro.begin();
}

void loop() {
    car.go(40);
    car.rotate(90);
}
