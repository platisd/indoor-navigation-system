# INS-node
The INS-node is the device that will allow us to pinpoint the location of our colleagues' desks.

## Setup your IDE
Follow the instructions found [here](https://wiki.wemos.cc/tutorials:get_started:get_started_in_arduino#using_git_version) to setup your Arduino IDE and enable it to compile and upload sketches for Wemos Mini D1.

To be able to compile for the ATtiny, please follow the instructions [here](https://github.com/SpenceKonde/ATTinyCore/blob/master/Installation.md).

## Power Controller :left_right_arrow: WiFi Module interaction
The Power Controller MCU is in charge of managing when the WiFi module is to be turned on. That being said, the WiFi module occasionally has some saying on what needs to be done.

There are two signals connecting the two components which additionally happen to be UART-capable. On the Power Controller these are `PB0` as `TX` and `PB1` as RX. On the WiFi module the standard `RX` and `TX` pins are utilized. Currently the two modules interact through sending signals as short (few microseconds) pulses.

The following table describes the protocol in the different states.

| Power Controller state | Source | Destination | Description |
| :----:|:----:|:----:|:----:|
| `SLEEP_WIFI_ON`          | Power Controller | WiFi Module  | Notifies WiFi Module to shutdown, transits to `PREPARE_SHUTDOWN` state where it will turn off after `WIFI_PREPARE_SHUTDOWN_DURATION`. |
| `SLEEP_WIFI_ON`          |  WiFi Module | Power Controller | Notifies Power Controller that it wants to shutdown and Power Controller transits to `PREPARE_SHUTDOWN` state where it will turn off after `WIFI_PREPARE_SHUTDOWN_DURATION`. |
| `PREPARE_SHUTDOWN`       |  WiFi Module | Power Controller | Notifies Power Controller to prevent shutdown and go back to `SLEEP_WIFI_ON` state. |

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
