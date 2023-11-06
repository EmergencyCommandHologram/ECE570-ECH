
/******************************************************************************
 *
 *                            Title: Assignment Wifi to Database
 *
 ******************************************************************************
 * Program Detail:   
 * FileName:           main.ccp
 * Purpose: To obtain sensor data and real time, and output to a database
 *      
 * Inputs: Switch, temperature sensor, ultrasonic sensor
 * Outputs: Ping to Ultrasonic sensor, http, and serial monitor
 * Date: 11/4/2023
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
#include "ledblink.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid     = "LG Mobile Network Gabe"; // must be exactly as it apears - case sensitive
const char* password = "EEGabe2371";
//const char* ssid     = "Nicholson Network"; // must be exactly as it apears - case sensitive
//const char* password = "plushy4u";

String url = "https://timeapi.io/api/Time/current/zone?timeZone=America/Los_Angeles";

DynamicJsonDocument doc(1024);

String sendtosite  = "https://gnicholsonee.000webhostapp.com/connect.php";
String data1   = "?node_name=node-1&time_received="; // currently empty
String data2  = "&temperature=";
String data3 = "&distance=";
String timereq;

const int trigPin = 13; // Trigger Pin D7
const int echoPin = 15; // Echo Pin D8
long duration, inches;
double Thermistor(int);
Blink led1; 
Blink led2; 

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
  
  //Tempsensor setting
  pinMode(12,INPUT);  //D6

  //Distance sensor settings
  pinMode(trigPin, OUTPUT); //D7
  pinMode(echoPin, INPUT); //D8

}

void loop() {
  
  //Peforming distance sensor info gathering
  digitalWrite(trigPin, LOW);             //
  delayMicroseconds(2);                   // 
  digitalWrite(trigPin, HIGH);            //
  delayMicroseconds(10);                  //
  digitalWrite(trigPin, LOW);             //
  duration = pulseIn(echoPin, HIGH);      //
  inches = duration / 74 / 2;             //
  /////////////////////////////////////////

  //Performing temperature info gathering///////
  int readVal= analogRead(A0);                //
  int temp =  Thermistor(readVal)*(3.3/5); //
  //Function contains math. Not calibrated btw//
  //This temp sensor has not been properly    //
  //configured,but that is not the point      //
  //////////////////////////////////////////////



//SWITCH IS PRESSED
  if (digitalRead(14) == HIGH) {
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
          mytime.remove(19);
          timereq = mytime;
        }
        https.end(); // end of this request
      } 
      else {
          Serial.printf("[HTTPS] Unable to connect\n");
      }
      //////////////////////END TIME GET ///////////////////////////////////////////////////////////////
      
      //Compile all data into one URL 
      //"https://gnicholsonee.000webhostapp.com/connect.php"+"?node_name=node-1&time_received="+"time"+
      String DBurl = sendtosite + data1 + timereq + data2 + temp + data3 + inches;

      /////////////////////SEND GET TO WEBSITE/////////////////////////////////////////////////////////
      Serial.println("Requesting: --> " + DBurl);
      if (https.begin(client, DBurl)) { 
        Serial.println("I sent the following:");
        Serial.println(DBurl);
        int httpCode = https.GET();
        
        Serial.println("Response code <--: " + String(httpCode)); // print response code: e.g.,:200
        https.end(); // end of this request    
      } 
      else {
          Serial.printf("[HTTPS] Unable to connect\n");
      }
    }
    //////////////////////END WEBSITE GET///////////////////////////////////////////////
    //Serial.println("Sending a message:");
    Serial.println("Inches: ");
    Serial.println(inches);
    Serial.println("Temp");
    Serial.print(temp);  
    /////WAIT 5 Seconds//////////  
    delay(5000); // delay between each REQUEST to the server
    
  }
  delay(1000);
}


// put function definitions here:
//Function code taken from https://www.instructables.com/Arduino-37-in-1-Sensors-Kit-Explained/
//From user: RGBFreak ------------------------------------------------------------------------/
//Added in different calculations to get better results on my part
double Thermistor(int RawADC) {
  double Temp;
  Temp = log(10000.0*((1024.0/RawADC-1))); 
  Temp = 1/(0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Temp = Temp - 273.15;            // Convert Kelvin to Celcius
  Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
  return Temp;
}