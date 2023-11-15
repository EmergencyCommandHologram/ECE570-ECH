
/******************************************************************************
 *
 *                            Title: API Assignment part 1
 *
 ******************************************************************************
 * Program Detail:   
 * FileName:           main.ccp
 * Purpose: To send a request to IFTTT
 *      
 * Inputs: None
 * Outputs: None
 * Date: 11/12/2023
 * Compiler: Visual Studio Code 1.82.2
 * Author: Gabriel Nicholson
 * Version: Rev 000
 *
 ******************************************************************************
 * File Dependencies: #include <Arduino.h>
 *   
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

String url = "https://timeapi.io/api/Time/current/zone?timeZone=America/Los_Angeles";

DynamicJsonDocument doc(1024);

String sendtosite  = "https://maker.ifttt.com/trigger/ESP_Get/with/key/";
String key = "cFt4iOXlUY8qlmfRkxV1Bu";


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


  //Switch settings
  pinMode(14,INPUT);  //D5
  pinMode(5, OUTPUT); //D1
  digitalWrite(5,HIGH); //D1

  pinMode(13, OUTPUT); //D7
  pinMode(15, INPUT); //D8

}

void loop() {
  
  digitalWrite(13,LOW);
//SWITCH IS PRESSED
  if (digitalRead(14) == HIGH) {
    digitalWrite(13,HIGH);
    //---------------------------------------------------------------------------------------------//
    //Wificonnection:
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClientSecure client;
      client.setInsecure();
      HTTPClient https;


    //////////////////GET TIME/////////////////////////////////////////////////////////////////////////
      String fullUrl = url; // preparing the full URL
      Serial.println("Requesting: --> " + fullUrl);

      if (https.begin(client, fullUrl)) { // start the connection 1=started / 0=failed

        int httpCode = https.GET();
 
        Serial.println("Response code <--: " + String(httpCode)); // print response code: e.g.,:200

        if (httpCode > 0) {
          deserializeJson(doc,https.getString()); // deserialize the JSON format into keys and values
          String mytime = doc["dateTime"]; // get the value associated with the dataTime key
          mytime.replace('T','_');
          mytime.remove(21);
          String timereq = mytime;
        }
        https.end(); // end of this request
      } 
      else {
          Serial.printf("[HTTPS] Unable to connect\n");
      }
      //////////////////////END TIME GET ///////////////////////////////////////////////////////////////
      
      String DBurl = sendtosite + key;

      /////////////////////SEND GET TO WEBSITE/////////////////////////////////////////////////////////
      Serial.println("Requesting: --> " + DBurl);
      if (https.begin(client, DBurl)) { 
        Serial.println("I sent the following:");
        Serial.println(DBurl);
        int httpCode = https.GET();
        
        Serial.println("Response code <--: " + String(httpCode)); // print response code: e.g.,:200
        Serial.println(https.getString());
        https.end(); // end of this request    
      } 
      else {
          Serial.printf("[HTTPS] Unable to connect\n");
      }
      delay(60000); // delay between each REQUEST to the server
    }
    //////////////////////END WEBSITE GET///////////////////////////////////////////////

    /////WAIT 5 Seconds//////////  
    
    
  }
  delay(1000);
}

