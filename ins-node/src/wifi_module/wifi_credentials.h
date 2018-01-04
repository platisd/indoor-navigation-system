// Do not version control this file, it should contain your private data
// To ignore this file while commiting use the following command:
// git update-index --skip-worktree path/to/this/file.h
// To undo (in case you want to switch branches or commit something):
// git update-index --no-skip-worktree path/to/this/file.h

#ifndef WIFI_MODULE_SKETCH_H
#define WIFI_MODULE_SKETCH_H

const char internetSSID[] = "SSID to connect";
const char password[] = "yourP4sSw0rD";
std::set<String> positioningNetworks = {"SSID-a", "SSID-b"};
const uint16_t HTTP_PORT = 8050; // ins-server port number
const char SERVER_IP[] = "192.168.0.2"; // ins-server IP address

#endif
