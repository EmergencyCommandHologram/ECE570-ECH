/******************************************************************************
 *
 *                            Title: Microcontroller Assignment Part 1
 *                                    Random Number Serial Plotter
 *
 ******************************************************************************
 * Program Detail:   
 * FileName:           RandomNumber_Arduino.ino
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

int y = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  delay(5000);
  y = random(0,100);
  Serial.println(y);
}
