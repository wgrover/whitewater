#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "secrets.h"

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASSWORD;

const char* server = "waterservices.usgs.gov/nwis/iv/?format=json&sites=10257548";
const int port = 443;

WiFiClientSecure client;

void setup() {
   Serial.begin(115200);
   WiFi.mode(WIFI_STA); 
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
   }
   Serial.println("");  Serial.print("WiFi connected to: "); Serial.println(ssid);  Serial.println("IP address: ");  Serial.println(WiFi.localIP());
   delay(2000);
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;
 
    http.begin("https://waterservices.usgs.gov/nwis/iv/?format=json&sites=10257548"); 
    int httpCode = http.GET();
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }
 
  delay(10000);
}
