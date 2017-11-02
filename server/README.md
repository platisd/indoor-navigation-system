# INS Server
The INS server provides a backend RESTful service for INS-node and front-end web or mobile applications.

## API
* Send RSSI value to INS server (INS node)

  INS node is required to send RSSI reading per wifi AP's MAC address to INS service. In a bid to handle outliers, the server supports sending multiple data points from INS node.

  HTTP Method - `POST`

  Request Url -  `/set_rss/device_id/mac_addr1/rssi1/mac_addr2/rssi2/mac_addr3/rssi3/mac_addr4/rssi4/:end?`

  Response - `{result:success}` or `{result:error}`

  #### Examples
  * Device `1000` sending a one-time RSSI reading for MAC addresses `aa`, `bb`, `cc` and `dd`.

    `http://localhost:9080/set_rss/1000/aa/12.323/bb/44.3434/cc/1.223/dd/54.43/end`

  * Device `5239` sending multiple RSSI readings for MAC addresses `aa`, `bb`, `cc` and `dd`.

    `http://localhost:9080/set_rss/5239/aa/12.323/bb/44.3434/cc/1.223/dd/54.43`
    `http://localhost:9080/set_rss/5239/aa/12.323/bb/44.3434/cc/1.223/dd/54.43`
    `http://localhost:9080/set_rss/5239/aa/12.323/bb/44.3434/cc/1.223/dd/54.43/end`

* Retrieve employee position (Frontend application)

  HTTP Method - `GET`

  Request Url - `/get_employee_position/:employee_id`

  Response - `employee_id:<id> {loc_x:<val>, loc_y:<val>, loc_z:<val>}`

  #### Example
  * Get position of employee with id = 8923

    `http://localhost:5300/get_employee_position/8923`


* Retrieve device position (Frontend application)

  HTTP Method - `GET`

  Request Url - `/get_device_position/:device_id`

  Response - `device_id:<id> {loc_x:<val>, loc_y:<val>, loc_z:<val>}`

  #### Example
  * Get position of device with id = 2020

    `http://localhost:5300/get_device_position/2020`


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
* To run use `./ins_server [port]`

## Testing
This component is tested using [GoogleTest](https://github.com/google/googletest/tree/master/googletest) and [GoogleMock](https://github.com/google/googletest/tree/master/googlemock) which are C++ testing and mocking frameworks.

The repository contains a [include/](include/) and [src/](src/) directories where the user is expected to maintain their production code and the [test/](test/) directory where the respective unit test and mocks have to be placed.


### How to run tests
* Build and run unit tests
  * `build_and_run_ut.sh`
* Build and run unit tests, cleaning the build folder
  * `build_and_run_ut.sh --clean`
