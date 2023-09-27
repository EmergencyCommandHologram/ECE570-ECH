/******************************************************************************
 *
 *                            Title: Microcontroller Assignment Part 1
 *                                    LED On/Off control
 *
 ******************************************************************************
 * Program Detail:   
 * FileName:           LEDkeyboard_frequency.ino
 * Purpose: To blink an LED at different speeds based on wheter the letter A or B is sent (for arduino ide)      
 * Inputs: Keyboard input
 * Outputs: Digital output to LED (pin D5), and serial monitor
 * Date: 9/23/2023
 * Compiler: Arduino IDE 2.2.1
 * Author: Gabriel Nicholson
 * Version: Rev 000
 *
 ******************************************************************************
 * File Dependencies: 
 *
 *****************************************************************************/
char readChar;
int freq = 1000;

void setup() {
  Serial.begin(9600); // Starts the serial communication
  pinMode(14, OUTPUT); // Sets the trigPin as an Output
}

int blink(int x) {
  delay(x);
  digitalWrite(14, HIGH);
  delay(x);
  digitalWrite(14, LOW);
  return 0;
}

void loop() {
  blink(freq);
  if (Serial.available() > 0) {
    char readChar = Serial.read();
    if (readChar == 'A') {
      freq = 100;
    }
    else if (readChar == 'B'){
      freq = 500;
    }
    else {
    }
  } 
}