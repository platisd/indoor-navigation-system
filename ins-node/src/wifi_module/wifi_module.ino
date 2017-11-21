/*
 *  This sketch demonstrates how to scan WiFi networks. 
 *  The API is almost the same as with the WiFi Shield library, 
 *  the most obvious difference being the different file you need to include:
 */
#include "ESP8266WiFi.h"

typedef struct ctx
{
  String MacAddress;
  byte RssiLevel;
}ctx_t;

#define device_id 1;

ctx_t contextMsg[6];
const char* ssid     = "TP-LINK_3351";
const char* password = "20791375";

const char* host = "192.168.0.136";

void setup() {
  Serial.begin(115200);
  memset(contextMsg,0,sizeof(ctx_t)*6);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected. Node");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Setup done");
}

void loop() {
  contextMsg[0].RssiLevel = -50;
  Serial.println("Start Scanning for APs");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("Scanning Completed!");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID, MAC address and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" : MAC => ");
      Serial.print(WiFi.BSSIDstr(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
      if ((contextMsg[0].RssiLevel < WiFi.RSSI(i)))
      {
        contextMsg[0].RssiLevel  = WiFi.RSSI(i);
        contextMsg[0].MacAddress = WiFi.BSSIDstr(i);
      }
    }
  }
  Serial.println("");

  if (1)
  {
     Serial.print("Connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 8050;
  if (!client.connect(host, httpPort)) {
    Serial.println("Rest server connection failed");
    return;
  }

    // We now create a PUT to the rest server
  String req = "/set_rssi/:";  ///set_rssi/:device_id/:mac_addr1/:rssi1/:mac_addr2?/:rssi2?/:mac_addr3?/:rssi3? ... /:mac_addr10?/:rssi10/?
  req += device_id;
  req += "/:";
  req += contextMsg[0].MacAddress;
  req += "/:";
  req += contextMsg[0].RssiLevel;
  req += "/?";
  
  Serial.print("Requesting Command: ");
  Serial.println(req);
  
  // Send req to the rest server
  client.print(String("PUT ") + req + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

    // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  }

  Serial.println("Closing connections...");
  //WiFi.disconnect();

  // Wait a bit before scanning again
  delay(5000);
}
