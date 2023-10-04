
/******************************************************************************
 *
 *                            Title: Microcontroller Assignment Part 1
 *                                    LED On/Off control
 *
 ******************************************************************************
 * Program Detail:   
 * FileName:           main.ccp
 * Purpose: To measure light level and activate a buzzer and change a LED's color 
 * based on the light level, as well as display the lux reading in the serial monitor.
 *      
 * Inputs: Photoresistor voltage, Keyboard input
 * Outputs: Digital outputs to LED (total of 3), output to buzzer (1) and serial monitor
 * Date: 9/30/2023
 * Compiler: Visual Studio Code 1.82.2
 * Author: Gabriel Nicholson
 * Version: Rev 000
 *
 ******************************************************************************
 * File Dependencies: Arduino.h
 *
 *****************************************************************************/
#include <Arduino.h>

char readChar;
float LuxVoltage = 0;
float RAW = 0;
float LuxConverted = 0;
float Duty = 0;
int tim = 0;

void setup() {
  Serial.begin(9600); // Starts the serial communication
  //RGB outputs
  pinMode(14, OUTPUT); //D5 Read
  pinMode(12, OUTPUT); //D6 Green 
  pinMode(13, OUTPUT); //D7 Blue
  //Buzzer output
  pinMode(5, OUTPUT); //D1 
  //Photoresistor input
  pinMode(2, INPUT);  //D4
  digitalWrite(14, HIGH);
  analogWrite(12, 255);
  analogWrite(13, 255);

}

void loop() {
  //Read in the analog voltage that has been converted from 0-3.3 to 0-1023
  RAW = analogRead(A0);
  LuxVoltage = RAW*(3.3/1024.0);

  LuxConverted = (14858.84*LuxVoltage - 1069.84);
  //duty decreases as Lux increases. Will be completely 0 when Lux is at 1000

  Duty = 256 - (LuxConverted/8);
  
  delay(100);
  if (tim > 20) {
    //Formula for lux derived from data
    Serial.println("The light reading is:");
    Serial.println(LuxConverted);
    //Serial.println("The Voltage is:");
    //Serial.println(LuxVoltage*(3.3/1024.0),4);
    tim = 0;
  }
  
  //Threshold for activating buzzer in low light
  if (LuxConverted < 200) {
    //if switches from high light, immediately to low light
    //buzzer activated for 5 pulses
    for (int i = 0; i < 5;) {
      digitalWrite(5, HIGH);
      delay(320);
      digitalWrite(5, LOW);
      delay(50);
      i++;
      Serial.println("The light reading is:");
      Serial.println(LuxConverted);
    }
  }

  //Threshold for activating red light in bright light
  else if (LuxConverted >= 800) {
    analogWrite(12, Duty);
    analogWrite(13, Duty);
  }
  //Otherwise make LED white
  else {
    digitalWrite(14, HIGH);
    analogWrite(12, 255);
    analogWrite(13, 255);
  }

  //Buzzer testing through serial
  if (Serial.available() > 0) {
    char readChar = Serial.read();
    if (readChar == 'B') {
        digitalWrite(5, HIGH);
        delay(5000);
        digitalWrite(5, LOW);
    }
  } 
  tim++;
}