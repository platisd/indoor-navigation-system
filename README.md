# Indoor Navigation System [![Build Status](https://travis-ci.org/platisd/indoor-navigation-system.svg?branch=master)](https://travis-ci.org/platisd/indoor-navigation-system) [![codecov](https://codecov.io/gh/platisd/indoor-navigation-system/branch/dev/graph/badge.svg)](https://codecov.io/gh/platisd/indoor-navigation-system)

[WIP] An indoor navigation system that guides you to the desk of your colleagues.

## What?
INS is a navigation system that utilizes a fleet of portable embedded devices, which have the ability to provide positioning information about themselves, to guide its user towards the desk of an employee or a colleague. The main scenario begins by searching for an employee on a mobile client. Then the system provides information regarding the whereabouts of the said employee's desk as well as navigational instructions on how to reach it.

The system does not intend to monitor individuals, but merely broadcast their currently designated workspace. The tracking nodes are not meant to be constantly carried along, but placed on the desk where someone is working on and forgotten until their owner changes desk.

## Why?
Medium and large organizations involve office settings where employees are spread out throughout different floors. It is typically difficult to locate personnel with whom there is infrequent physical collaboration. The challenge becomes even greater when employees either do not possess a dedicated work-area or tend to move between locations. The main advantage of INS is that it provides dynamic updates in case the position of an employee's workspace changes, voiding the need for any manual action from  the employee's side.

What is possibly more important is the technology developed within this context, can be easily reused in different occasions, such as in IoT and indoor positioning applications.

## How?
A fleet of portable devices, transmit the signal strength (RSSI) of their surrounding WiFi access points (AP) to a central server. Since the exact location of the various APs is known, we can utilize the provided data to pinpoint the nodes within the building. Finally, a user can fetch the calculated location from the server using a mobile client.

INS is a distributed system composed of the following layers that communicate via HTTP requests:
* [Node](/ins-node)
  * A portable device that transmits the signal strengths it detects. It is comprised of a WiFi Module that handles the connectivity and a Power Module that determines when the node should go to sleep to minimize its power consumption.
* [Server](/server)
  * A REST server that receives the RSSI signals, calculates the location of each node and supplies the information to the interested parties.
* [Client](/ins-app)
  * A mobile client that visualizes the location of the inquired employee and guides its user towards that location.
