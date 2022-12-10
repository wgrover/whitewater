// Calibration with 10 ohm resistor:
// ledcWrite   Current [mA]
//       0            0
//      32          5.2
//      64         10.5
//      96         15.9
//     128         20.8
//     160         26.0
//     192         31.1
//     224         36.3
//     255         41.2

// River flow rate over the last several years maxes out a little above 1000 ft^3/s

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <math.h>
#include "secrets.h"

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASSWORD;
const char* server = "waterservices.usgs.gov/nwis/iv/?format=json&sites=10257548";
const int port = 443;
const int ledPin = 16;  // GPIO16, connected to ammeter via 10 ohm resistor

// PWM properties:
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

const float slope = 6.1921;
const float intercept = -0.7697;

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
  pinMode(2, OUTPUT);
}

void loop() {
  digitalWrite(2, HIGH);
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin("https://waterservices.usgs.gov/nwis/iv/?format=json&sites=10257548");
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.print("http=" + String(httpCode) + "\t");
      // Serial.println(payload);
      DynamicJsonDocument doc(10000);
      deserializeJson(doc, payload);
      double flow = doc["value"]["timeSeries"][0]["values"][0]["value"][0]["value"];
      Serial.print("flow=" + String(flow) + "\t");
      int pwm = 0;
      if (flow < 0.1) {
        pwm = 0;
      }
      else if (flow > 1000.0) {
        pwm = 255;
      }
      else {
        pwm = round( slope * ((log10(flow)+1)*10) + intercept );
      }
      Serial.println("PWM=" + String(pwm));
      ledcWrite(ledChannel, pwm);
    }
    else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }
  digitalWrite(2, LOW);
  delay(60000);  // updating every minute
}
