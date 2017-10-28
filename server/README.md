# INS Server
The INS server provides a backend RESTful service for INS-node and front-end web or mobile applications.

## Dependencies
* [Pistache](http://pistache.io/)
* [Sqlite3](https://www.sqlite.org/)
* [spdlog](https://github.com/gabime/spdlog)

### How To Fetch Dependencies
#### Ubuntu
* Install SQLite3
    * `sudo apt-get install sqlite3 libsqlite3-dev -y`
* Install Pistache
    * `git clone https://github.com/oktal/pistache.git`
    * `cd pistache`
    * `mkdir build && cd build`
    * `cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..`
    * `make`
    * `sudo make install`

## Build & Run Backend Server
Ensure all needed libraries are installed.
* `cd server`
* `mkdir build`
* `cd build`
* `cmake ..`
* `make`
* To run use `./ins_server`

## Testing
This component is tested using [GoogleTest](https://github.com/google/googletest/tree/master/googletest) and [GoogleMock](https://github.com/google/googletest/tree/master/googlemock) which are C++ testing and mocking frameworks.

The repository contains a [include/](include/) and [src/](src/) directories where the user is expected to maintain their production code and the [test/](test/) directory where the respective unit test and mocks have to be placed.


### How to run tests
* Build and run unit tests
  * `build_and_run_ut.sh`
* Build and run unit tests, cleaning the build folder
  * `build_and_run_ut.sh --clean`
