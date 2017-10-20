# Indoor Navigation System [![Build Status](https://travis-ci.org/platisd/indoor-navigation-system.svg?branch=master)](https://travis-ci.org/platisd/indoor-navigation-system)

[WIP] An indoor navigation system to guide users towards their colleagues' desks

## Dependencies
* [Pistache](http://pistache.io/)
* [Sqlite3](https://www.sqlite.org/)
* [spdlog](https://github.com/gabime/spdlog)

### How To Fetch Dependencies
#### Ubuntu
* Do update
    * `sudo apt-get update`
* Install spdlog
    * `sudo apt-get install libspdlog-dev -y`  
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
