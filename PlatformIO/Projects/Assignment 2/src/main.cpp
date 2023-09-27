
/******************************************************************************
 *
 *                            Title: Microcontroller Assignment Part 1
 *                                    LED On/Off control
 *
 ******************************************************************************
 * Program Detail:   
 * FileName:           LEDkeyboard.ino
 * Purpose: To activate an LED based on wheter the letter A or B is sent (for arduino ide)      
 * Inputs: Keyboard input
 * Outputs: Digital output to LED (pin D5), and serial monitor
 * Date: 9/23/2023
 * Compiler: Arduino IDE 2.2.1
 * Author: Gabriel Nicholson
 * Version: Rev 000
 *
 ******************************************************************************
 * File Dependencies: Arduino.h
 *
 *****************************************************************************/
#include <Arduino.h>

char readChar;
void setup() {
  Serial.begin(9600); // Starts the serial communication
  pinMode(14, OUTPUT); // Sets the trigPin as an Output
}

void loop() {
  if (Serial.available() > 0) {
    char readChar = Serial.read();
    if (readChar == 'A') {
      digitalWrite(14, HIGH);
      Serial.print("\nLED ON");
    }
    else if (readChar == 'B'){
      digitalWrite(14, LOW);
      Serial.print("\nLED OFF");
    }
    else {
      Serial.print("\nNot a valid character");
    }
  } 
}