/******************************************************************************
 *
 *                            Title: Microcontroller Assignment Part 1
 *                                    LED On/Off control
 *
 ******************************************************************************
 * Program Detail:   
 * FileName:           sketch_sep22a.ino
 * Purpose: To generate random numbers between 0 and 100 and output them to a serial plotter (for arduino ide)      
 * Inputs: Random number, no physical inputs 
 * Outputs: Serial plotter, no physical outputs
 * Date: 9/22/2023
 * Compiler: Arduino IDE 2.2.1
 * Author: Gabriel Nicholson
 * Version: Rev 000
 *
 ******************************************************************************
 * File Dependencies: None
 *
 *****************************************************************************/

void setup() {
  Serial.begin(9600); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {
  
  digitalWrite(trigPin, HIGH);
  digitalWrite(trigPin, LOW);
  
}