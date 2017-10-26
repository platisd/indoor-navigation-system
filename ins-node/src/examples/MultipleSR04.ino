#include <Smartcar.h>

SR04 front, back;

unsigned int d1 = 0;
unsigned int d2 = 0;

void setup() {
  front.attach(3, 4); //trigger pin, echo pin
  back.attach(6,7);
}

void loop() {
  d1 = front.getDistance();
  d2 = back.getDistance();
}
