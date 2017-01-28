# Smartcar Mock
A testing and mocking framework for the [Smartcar](http://plat.is/smartcar) Arduino library. You can use this to unit test your Smartcar sketches.

It is based on [GoogleTest](https://github.com/google/googletest/tree/master/googletest) and [GoogleMock](https://github.com/google/googletest/tree/master/googlemock) which are some easy-to-use C++ testing and mocking frameworks. It builds upon the [Arduino-Mock](https://github.com/ikeyasu/arduino-mock) project by Anders Arnholm and Yasuki Ikeuchi.

The repository contains a [src/](src/) directory where the user is expected to maintain their production code and the [test/ut/](test/ut/) directory where the respective unit tests (\*.cc files) have to be placed. Inside each of them, you will find examples ([1](src/examples/), [2](test/ut/examples/)) folders with some ready to use production code and unit tests.

## Useful reading
* [Introduction to GoogleTest](https://github.com/google/googletest/blob/master/googletest/docs/Primer.md)
* [Introduction to GoogleMock](https://github.com/google/googletest/blob/master/googlemock/docs/ForDummies.md)
* [Smartcar API Reference](https://github.com/platisd/smartcar_shield/wiki/API-documentation)

## Dependencies
* git
* make
* cmake
* C++ compiler

## How to clone, build and run
* Clone the project
`git clone https://github.com/platisd/smartcar-gmock.git`
* Inialize the `smartcar_mock` submodule
`git submodule update --init`
* Build and run the unit tests
`./build_and_run_ut.sh`

## How to get started with unit testing
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

```C++
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

```C++
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

```C++
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

## Proposed workflow
Ideally, you should use this repository to develop your code in this already set up testing environment. It is strongly suggested that you do not delete the version controlled history of the files that are found here, so you can make changes and receive updates (through the smartcar_mock submodule) in the future easier, if the need arises.

Depending on whether you are starting from scratch or already have a repository set up, you should follow [Method A](#method-a-mirroring-the-smartcar-gmock-repository) or [Method B](#method-b-merging-with-the-smartcar-gmock-repository) respectively, which are outlined below. After you are done with either of the two methods, do not forget to initialize the submodule with `git submodule update --init` before starting to test.

### Method A: Mirroring the smartcar-gmock repository
**Warning:** Mirror pushing to an already existing repository will overwrite your previous work. If that is the case, please follow [Method B](#method-b-merging-with-the-smartcar-gmock-repository) instead.

If you are starting from scratch, you can create a duplicate of this repository and move on with your development. To do this, you will need to run the following steps adopted from GitHub's "[Duplicating a repository](https://help.github.com/articles/duplicating-a-repository/#mirroring-a-repository)":
* Create a bare clone of the repository

`git clone --bare https://github.com/platisd/smartcar-gmock.git`
* Mirror-push to the new repository

`cd old-repository.git`

`git push --mirror https://github.com/exampleuser/my-smartcar-project.git`
* Remove the temporary local repository you previously created
`cd ..`

`rm -rf smartcar-gmock.git`

### Method B: Merging with the smartcar-gmock repository
Follow these instructions if you have set up a working repository, which you do not want to overwrite and lose.

* Add the smartcar-gmock repository as a remote into your already existing repository

If for example your repository is [DIT524-V17/group-99](https://github.com/DIT524-V17/group-99) then you should do the following:

`git clone https://github.com/DIT524-V17/group-99.git`

`cd group-99`

`git remote add -f smartcar-gmock https://github.com/platisd/smartcar-gmock.git`
* Merge the smartcar-gmock repository with your already existing work

`git merge smartcar-gmock/master --allow-unrelated-histories`

Or if you are using an old git version and the command above does not work, simply run the same without the flag:

`git merge smartcar-gmock/master`
* Resolve possible merge conflicts
E.g. on the README or LICENSE files

* Add the resolved files when you are done and commit the merge

## How to update to the latest mock classes
An effort will be made to keep this repository stable, with as little (substantial) changes as possible. So after you clone this, you should not need to pull again. This is done to enable the ones who have cloned it in the past, to seamlessly develop and maintain their production code in the `src/` directory. Whatever important changes that could be introduced will be through the `smartcar_mock` submodule that can be independently updated by running without having a dependency to this repository, by running:

`git submodule foreach git pull --rebase origin master`
