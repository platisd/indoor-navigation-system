#include <stdint.h>
#include <math.h>
#include <vector>
#include <utility>
#include <set>
#include <ESP8266WiFi.h>
#include "wifi_credentials.h"

ESP8266WiFiClass wifi;
EspClass espPower;
const uint8_t DEVICE_ID = 1; // The identification for the specific node
const int8_t CONNECTION_RETRIES = 20; // The maximum amount of wifi connection attempts
const uint8_t DEFAULT_TRANSMISSION_SIZE = 10;
uint8_t currentTransmissionSize = DEFAULT_TRANSMISSION_SIZE;

const uint8_t TX_PIN = 10;
const uint8_t RX_PIN = 9;

/**
   Gets the network context (i.e. MAC address and RSSI value) of the specified
   network item.
   @param networkItem The network item index of which to fetch the context
   @return            A tuple of (MAC address, RSSI)
*/
std::pair <String, int32_t> getNetworkContext(int8_t networkItem) {
  std::pair <String, int32_t> context (wifi.BSSIDstr(networkItem), wifi.RSSI(networkItem));
  return context;
}

/**
   Measures the RSSI values for the various access points of the specified SSID
   and returns them along the MAC address of the respective access point.
   @return  The MAC addresses with their respective RSSI values
*/
std::vector<std::pair <String, int32_t>> getDatapoints() {
  std::vector<std::pair <String, int32_t>> datapoints;
  // Scan the surrounding networks and get how many were found
  auto networksFound = wifi.scanNetworks();
  for (auto i = 0; i < networksFound; ++i) {
    // Check if some of the networks should be used for positioning
    bool validNetwork = positioningNetworks.find(wifi.SSID(i)) != positioningNetworks.end();
    if (validNetwork) {
      datapoints.push_back(getNetworkContext(i));
    }
  }

  return datapoints;
}

/**
   Transmits the supplied data (if present) to the server
   @param  datapoints The MAC addresses and the RSSI values we collected
   @return            Whether transmission succesfull
*/
bool transmitData(std::vector<std::pair <String, int32_t>> datapoints) {
  // TO-DO: Notify the server that we could not gather any datapoints for
  // corrective actions to be made before sleeping.
  if (datapoints.empty()) {
    return false;
  }
  // Establish a TCP connection to the server
  WiFiClient client;
  if (!client.connect(SERVER_IP, HTTP_PORT)) {
    return false;
  }

  // Compose a request for the server according to the agreed format:
  // /set_rssi/device_id/mac_addr1/rssi1/mac_addr2/ ... /mac_addr10/:rssi10/
  String request = "/set_rssi/";
  request += DEVICE_ID;
  request += "/";
  for (auto datapoint : datapoints) {
    // We don't put everything in the same expression since std::string
    // (used in testing) is not exactly the same as Arduino String. Therefore
    // we go with this way of concatinating the string which is compatible with
    // both implementations.
    request += datapoint.first; // The MAC address
    request += "/";
    request += datapoint.second; // The RSSI value
    request += "/";
  }

  // Send the TCP request to the rest server
  client.print("POST " + request + " HTTP/1.1\r\n" +
               "Host: " + SERVER_IP + "\r\n" +
               "Connection: close\r\n\r\n");

  // Verify whether the request timed out
  bool successfulRequest = true;
  const unsigned long TIMEOUT = millis() + 5000;
  while (client.available() == 0) {
    if (millis() > TIMEOUT) {
      successfulRequest = false;
      break;
    }
  }
  client.stop();
  return successfulRequest;
}

/**
   Puts the module into deep sleep and signals the power controller to
   turn its power off.
*/
void goToSleep() {
  digitalWrite(TX_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TX_PIN, LOW);
  espPower.deepSleep(60 * 1000000UL); // Will sleep forever unless wakeup pin connected to RST
}

void setup() {
  // Setup IO pins.
  pinMode(TX_PIN, OUTPUT);
  pinMode(RX_PIN, INPUT);
  digitalWrite(TX_PIN, LOW);

  // Set WiFi to station mode and disconnect from an AP
  // if it was previously connected.
  wifi.mode(WIFI_STA);
  wifi.disconnect();
  delay(100);

  // Try to connect to the internet.
  wifi.begin(internetSSID, password);
  auto attemptsLeft = CONNECTION_RETRIES;
  while ((wifi.status() != WL_CONNECTED) && (--attemptsLeft > 0)) {
    delay(500); // Wait a bit before retrying
  }
  // If there were problems during connection, go to deep sleep.
  if (attemptsLeft <= 0) {
    goToSleep();
  }
}

void loop() {
  // Discover the access points we are interested in to log down their
  // MAC addresses and RSSI values.
  auto datapoints = getDatapoints();

  // We need to split the data in groups of 10, as this is the maximum amount
  // we have agreed to send per transmission.
  uint8_t transmissions = ceil(datapoints.size() / static_cast<double>(currentTransmissionSize));
  for (auto i = 0; i < transmissions; i++) {
    // Get the next currentTransmissionSize elements as a new vector
    std::vector<std::pair<String, int32_t>> transmissionData(
                                           datapoints.begin() + (i * currentTransmissionSize),
                                           datapoints.end() - ((transmissions - (i + 1)) * currentTransmissionSize));
    // Transmit the collected data.
    bool transmissionWasSuccess = transmitData(datapoints);
    if (!transmissionWasSuccess) {
      // TO-DO: Decide what to do depending on transmission failure
      break;
    }
  }
  // Release resources and go to sleep
  wifi.disconnect();
  goToSleep();
}
