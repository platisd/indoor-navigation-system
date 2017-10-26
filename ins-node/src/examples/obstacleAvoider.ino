#include <Smartcar.h>

Gyroscope gyro;
Car car;
SR04 sensor;

void setup() {
    gyro.attach();
    gyro.begin();
    sensor.attach(3,4);
    car.begin(gyro);
}

void loop(){
    unsigned int distance = sensor.getDistance();
    if (distance && distance < 20){
        car.setSpeed(100);
        car.rotate(90);
    } else {
        car.setSpeed(50);
    }
}
