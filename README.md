# Smartcar Mock
A testing and mocking framework for the [Smartcar](http://plat.is/smartcar) Arduino library. You can use this to unit test your Smartcar sketches.

It is based on [GoogleTest]https://github.com/google/googletest/tree/master/googletest) and [GoogleMock](https://github.com/google/googletest/tree/master/googlemock) which are some easy-to-use C++ testing and mocking frameworks. It builds upon the [Arduino-Mock](https://github.com/ikeyasu/arduino-mock) project by Anders Arnholm and Yasuki Ikeuchi.

The repository contains a [src/](src/) directory where the user is expected to maintain their production code and the [test/ut/](test/ut/) directory where the respective unit tests (\*.cc files) have to be placed. Inside each of them, you will find examples ([1(src/examples/)], [2(test/ut/examples/)]) folders with some ready to use production code and unit tests.

## Useful reading
* [Introduction to GoogleTest](https://github.com/google/googletest/blob/master/googletest/docs/Primer.md)
* [Introduction to GoogleMock](https://github.com/google/googletest/blob/master/googlemock/docs/ForDummies.md)

## How to get started
The simplest way to getting started with the Smartcar Mock framework is by using the examples already included with this repository. For instance, let's have a look at a simple obstacle avoider sketch that can be found in [src/examples/obstacleAvoider.ino](src/examples/obstacleAvoider.ino). Its unit tests can be found at [test/ut/examples/ObstacleAvoider_test.cc](test/ut/examples/ObstacleAvoider_test.cc).

```arduino
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
```

First, the sketch simply instructs the Smartcar to read a distance from the attached ultrasonic sensor. Then, if that distance is smaller than 20 centimeters, it sets the speed to 100% and rotates the car 90 degrees clockwise. If no distance is detected or the distance is larger than two, it sets the speed to 50%.

In order to test this sketch, we first need to make sure that all the components we need to use (the car, the ultrasonic sensor and the gyroscope) were initialized properly in the `void setup()` function. Please refer to the [documentation](https://github.com/platisd/smartcar_shield/wiki/API-documentation) of the Smartcar library on how to use the classes in reference.

To ensure that, we make use of the following unit test found in [test/ut/examples/ObstacleAvoider_test.cc](test/ut/examples/ObstacleAvoider_test.cc):

```
TEST_F(SmartcarObstacleAvoiderFixture, initsAreCalled) {
    EXPECT_CALL(*carMock, begin());
    EXPECT_CALL(*gyroscopeMock, attach());
    EXPECT_CALL(*gyroscopeMock, begin(_));
    EXPECT_CALL(*SR04_mock, attach(_, _));
    setup();
}
```

This will assure that the `begin` method of the Car class will be called, the `attach` of Gyroscope as well its `begin` with an argument that we do not care about its specific value and finally the `attach` method of the SR04 sensor, with two arbitrary arguments. Note that for simplicity reasons, exceptionally in regards to the `begin()` method of the Car class, you do not have to specify its arguments at all in the test.

That being said, the test would fail (or more accurately would not compile) if you had forgotten to provide the Gyroscope instance as an argument, since the car needs a gyroscope in order to rotate according the specified degrees. This logical error detection on compile time is a commodity that you would not normally have while compiling for the Arduino target. Therefore these tests can really help you discover small and hard-to-spot omissions that could render the whole system unusable. There are more safeguards like this, i.e. the various sensors cannot be used unless they are attached first. If you have not attached a sensor first in your production code and try to use it directly, the test will fail.

Next, we need to test the loop function, where we can identify two separate cases depending on the distance that is detected. If the detected distance is smaller than 20 centimeters, the car's speed should be set to 100% and it to rotate once 90 degrees clockwise.

```arduino
TEST_F(SmartcarObstacleAvoiderFixture, givenObstacleFound_rotates) {
    EXPECT_CALL(*SR04_mock, getDistance())
    .WillOnce(Return(5));
    EXPECT_CALL(*carMock, setSpeed(100));
    EXPECT_CALL(*carMock, rotate(90));
    loop();
}
```
By running `loop()` one time, this unit test expects the SR04's `getDistance` method to be called once and it to return 5 centimeters as result. Therefore, if the code we have written is correct, we expect the car's speed to be set to 100 with `setSpeed(100)` and it to rotate 90 degrees clockwise with `rotate(90)`.

Once we have ensured that our car performs as expected in this scenario, we need to also test the alternative case, according to which the ultrasonic sensor's reading is larger than 20 centimeters, with the unit test that can be found right below. Can you read it and explain the code yourself?

```arduino
TEST_F(SmartcarObstacleAvoiderFixture, noObstacleFound_slowsDown) {
    EXPECT_CALL(*SR04_mock, getDistance())
    .WillOnce(Return(70));
    EXPECT_CALL(*carMock, setSpeed(50));
    loop();
}
```

To build and run the specific tests that were explained above copy or move the [example production code](src/examples/obstacleAvoider.ino) and its [tests](test/ut/examples/ObstacleAvoider_test.cc) one folder up from where they are currently located. You should eventually have the following structure in your project:
`src/obstacleAvoider.ino`
`test/ut/ObstacleAvoider_test.cc`

Then simply run `./build_and_run_ut.sh` to get the test results.

![Test results](http://i.imgur.com/w2vkllp.png)

## How to run
`./build_and_run_ut.sh`

## Dependencies
* make
* cmake
* C++ compiler
