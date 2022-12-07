#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "secrets.h"

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASSWORD;
const char* server = "waterservices.usgs.gov/nwis/iv/?format=json&sites=10257548";
const int port = 443;

// the number of the LED pin
const int ledPin = 16;  // 16 corresponds to GPIO16

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

WiFiClientSecure client;

void setup() {
   Serial.begin(115200);
   WiFi.mode(WIFI_STA); 
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
   }
   Serial.println("");
   Serial.print("WiFi connected to: ");
   Serial.println(ssid);
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());
   delay(2000);
   ledcSetup(ledChannel, freq, resolution);
   ledcAttachPin(ledPin, ledChannel);
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
 
    HTTPClient http;
 
    http.begin("https://waterservices.usgs.gov/nwis/iv/?format=json&sites=10257548"); 
    int httpCode = http.GET();
    if (httpCode > 0) {
        String payload = http.getString();
        Serial.println(httpCode);
        // Serial.println(payload);
        DynamicJsonDocument doc(10000);
        deserializeJson(doc, payload);
        double sensor = doc["value"]["timeSeries"][0]["values"][0]["value"][0]["value"];
        int pwm = 255.0 * sensor/200.0;
        Serial.println(pwm);
        ledcWrite(ledChannel, pwm); 
      }
    else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }
 
  delay(10000);
}
