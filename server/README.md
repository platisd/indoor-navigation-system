# INS Server
The INS server provides a backend RESTful service for INS-node and front-end web or mobile applications.

## API
* Send RSSI value to INS server (INS node)

  INS-nodes are required to send RSSI readings per wifi AP's mac address to INS-service. In a bid to have accurate readings and handle outliers, the INS-server supports sending multiple data points from INS-nodes. A minimum of 1 data point and maximum of 10 data points can be sent in one http request, while unlimited number of requests can be used in sending readings.

  * HTTP Method - `POST`
  * Request Url -  `/set_rssi/:device_id/:mac_addr1/:rssi1/:mac_addr2?/:rssi2?/:mac_addr3?/:rssi3? ... /:mac_addr10?/:rssi10/?`
  * Response - `{result:success}` or `{result:error}`

    #### Examples
  * INS-node with device id 1000 sends a one-time RSSI reading for MAC addresses 23:43:3d:3e:5e:f5 and  5a:4e:44:ff:5a:6e.

    `http://localhost:9080/set_rssi/1000/23:43:3d:3e:5e:f5/12.323/5a:4e:44:ff:5a:6e/44.3434`

  * INS-node with device id 5239 sends multiple RSSI readings from MAC addresses 23:43:3d:3e:5e:f5, `5a:4e:44:ff:5a:6e with different requests at different times.

    `http://localhost:9080/set_rssi/5239/23:43:3d:3e:5e:f5/13/5a:4e:44:ff:5a:6e/40`
    `http://localhost:9080/set_rssi/5239/23:43:3d:3e:5e:f5/11.3/5a:4e:44:ff:5a:6e/44.3`
    `http://localhost:9080/set_rssi/5239/23:43:3d:3e:5e:f5/15.6/5a:4e:44:ff:5a:6e/44.4`

* Trigger position calculation for device.

  Since INS-service supports data submission in batches, calculation of the INS-node position from RSSI readings is only done when the device prompts the server.
  * HTTP Method - `POST`
  * Request url - `/resolve_pos/:device_id`
  * Response - `{result:success}` or `{result:error}`

    #### Example
  * INS-node with device id 5239 triggers device position computation after sending several RSSI readings.

    `http://localhost:9080/resolve_pos/5239`


* Clear stored RSSI data for a device.

  In a scenario when an INS-node is moved from one place to the other, it becomes needed to promt the server to clear all previously stored data readings which automatically becomes invalid due to the move.
  * HTTP Method - 'POST'
  * Request url - `/reset_pos/:device_id`
  * Response - `{result:success}` or `{result:error}`

    #### Example

  * INS-node with device id '3331' resets its position.

    `http://localhost:5300/reset_pos/3331`

* Retrieve employee position (Frontend).

  This is for user facing applications interested in fetching employee's position.
  * HTTP Method - `GET`
  * Request Url - `/get_employee_pos/:employee_id`
  * Response - `{employee_id:<id>, pos_x:<val>, pos_y:<val>, pos_z:<val>}`

    #### Example
  * Get position of employee with id = 8923

    `http://localhost:5300/get_employee_pos/8923`

* Retrieve device position (Frontend).

  This is for user facing applications interested in fetching position of an INS-node devicce.
  * HTTP Method - `GET`
  * Request Url - `/get_device_pos/:device_id`
  * Response - `{device_id:<id>, pos_x:<val>, pos_y:<val>, pos_z:<val>}`

    #### Example

  * Get position of device with id = 2020

    `http://localhost:5300/get_device_pos/2020`


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
