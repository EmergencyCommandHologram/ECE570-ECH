/******************************************************************************
 *
 *                            Title: Assignment Wifi to Database
 *
 ******************************************************************************
 * Program Detail:   
 * FileName:           ledblink.h
 * Purpose: The initialization of the class Blink and it's variables+functions
 *      
 * Inputs: N/A
 * Outputs: N/A
 * Date: 11/4/2023
 * Compiler: Visual Studio Code 1.82.2
 * Author: Gabriel Nicholson
 * Version: Rev 000
 *
 ******************************************************************************
 * File Dependencies: #include <Arduino.h>                 
 *
 *****************************************************************************/
#include <Arduino.h>

class Blink { 
    private:
        int _pin;
        int rate; 
        bool blinkstate;
        unsigned long previousMillis;

    public:
        Blink(); 
        void blinkRate(int rate); 
        void blinkpin(int pin);
        void blinkLED(bool blinkstate); 
};

