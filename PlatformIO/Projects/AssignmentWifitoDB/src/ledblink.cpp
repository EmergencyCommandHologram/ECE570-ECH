/******************************************************************************
 *
 *                            Title: Assignment Wifi to Database
 *
 ******************************************************************************
 * Program Detail:   
 * FileName:           ledblink.ccp
 * Purpose: define the methods of ledblink header
 *      
 * Inputs: N/A
 * Outputs: N/A
 * Date: 11/4/2023
 * Compiler: Visual Studio Code 1.82.2
 * Author: Gabriel Nicholson
 * Version: Rev 000
 *
 ******************************************************************************
 * File Dependencies: #include "ledblink.h"                    
 *
 *****************************************************************************/
#include "ledblink.h"
Blink::Blink() { 
    _pin = -1; 
    rate = 1000;
    blinkstate = false;
    previousMillis = 0;
}

void Blink::blinkpin(int pinnum) {
    _pin = pinnum; 
    pinMode(_pin, OUTPUT); 
}

void Blink::blinkRate(int ratenum) {
    rate = ratenum; 
}

void Blink::blinkLED(bool state) {
    blinkstate = state; 
    unsigned long currentMillis = millis();
    if (blinkstate == 1) {
        if (rate <= 0 || _pin <= 0) {
            Serial.println("Invalid pin or rate!"); 
        } 
        else if (currentMillis - previousMillis >= rate) {
                previousMillis = currentMillis;
                digitalWrite(_pin, !digitalRead(_pin));
        }
    }
}
