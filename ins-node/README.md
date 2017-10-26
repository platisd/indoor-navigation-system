# INS-node
The INS-node is the device that will allow us to pinpoint the location of our colleagues' desks.

## Testing
To test the INS-node code we base our work on the [Smartcar-gmock](https://github.com/platisd/smartcar-gmock) framework.

It is based on [GoogleTest](https://github.com/google/googletest/tree/master/googletest) and [GoogleMock](https://github.com/google/googletest/tree/master/googlemock) which are some easy-to-use C++ testing and mocking frameworks. It builds upon the [Arduino-Mock](https://github.com/ikeyasu/arduino-mock) project by Anders Arnholm and Yasuki Ikeuchi.

The repository contains a [src/](src/) directory where the user is expected to maintain their production code and the [test/ut/](test/ut/) directory where the respective unit tests (\*.cc files) have to be placed.

The main challenge with testing this is that the Arduino library was not created with testability in mind. There are already plenty of mocks, however they are not enough to cover the whole API. In case more mocks **need** to be added, we can use our local [fork](https://github.com/platisd/arduino-mock) to do so. Alternatively, functions, registers and variables should be stubbed when their mocking is not crucial for the testing process.

### How to run tests
* Build and run unit tests
  * `build_and_run_ut.sh`
* Build and run unit tests, cleaning the build folder
  * `build_and_run_ut.sh --clean`
