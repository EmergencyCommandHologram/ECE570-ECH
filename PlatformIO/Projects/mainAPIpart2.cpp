
/******************************************************************************
 *
 *                            Title: API Assignment Part2
 *
 ******************************************************************************
 * Program Detail:   
 * FileName:           main.ccp
 * Purpose: To obtain data from webpage if LEDs are on or off
 *      
 * Inputs: None
 * Outputs: 2 LEDs
 * Date: 11/13/2023
 * Compiler: Visual Studio Code 1.82.2
 * Author: Gabriel Nicholson
 * Version: Rev 000
 *
 ******************************************************************************
 * File Dependencies: #include <Arduino.h>
 *                    #include "ledblink.h"
 *                    #include <ESP8266WiFi.h>
 *                    #include <ESP8266HTTPClient.h>
 *                    #include <ArduinoJson.h>
 *
 *****************************************************************************/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid     = "LG Mobile Network Gabe"; // must be exactly as it apears - case sensitive
const char* password = "EEGabe2371";
//const char* ssid     = "Nicholson Network"; // must be exactly as it apears - case sensitive
//const char* password = "plushy4u";

String url = "https://gnicholsonee.000webhostapp.com/results.txt";

DynamicJsonDocument doc(1024);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //Wifi initialization///////////////////////////
  Serial.println("Connecting to WiFi"); 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print("-");
  }
  ////////////////////////////////////////////////
  pinMode(13, OUTPUT); //D7
  pinMode(15, OUTPUT); //D8

}

void loop() {
  
  //Wificonnection:
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient https;

    String fullUrl = url; // preparing the full URL
    Serial.println("Requesting: --> " + fullUrl);

    if (https.begin(client, fullUrl)) { // start the connection 1=started / 0=failed

      int httpCode = https.GET();

      Serial.println("Response code <--: " + String(httpCode)); // print response code: e.g.,:200

      if (httpCode > 0) {
        deserializeJson(doc,https.getString()); // deserialize the JSON format into keys and values
        String LED1state = doc["LED1"]; 
        String LED2state = doc["LED2"];
        Serial.println(LED1state);
        Serial.println(LED2state);

        if (LED1state == "on") {
          digitalWrite(13, HIGH);
        }
        else {
          digitalWrite(13, LOW);
        }
        if (LED2state == "on") {
          digitalWrite(15, HIGH);
        }
        else {
          digitalWrite(15, LOW);
        }
      }
      https.end(); // end of this request
    } 
    else {
        Serial.printf("[HTTPS] Unable to connect\n");
    }
    
    delay(90000); // delay between each REQUEST to the server
  }
}

