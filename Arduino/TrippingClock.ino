#include <Arduino.h>
#include "A4988.h"

// The pin mapping is for D1 mini ESP8266
// Dx --> GPIOxx
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15



int Step = D8; //GPIO14---D5 of Nodemcu--Step of stepper motor driver
int Dire  = D4; //GPIO2---D4 of Nodemcu--Direction of stepper motor driver
int Sleep = D7; //GPIO12---D6 of Nodemcu-Control Sleep Mode on A4988
int MS1 = D0; //GPIO13---D7 of Nodemcu--MS1 for A4988
int MS2 = D5; //GPIO16---D0 of Nodemcu--MS2 for A4988
int MS3 = D6; //GPIO15---D8 of Nodemcu--MS3 for A4988

//Motor Specs
const int spr = 200; //Steps per revolution
int RPM = 8; //Motor Speed in revolutions per minute - Here we set the RPM to 1 rpm to match the speed of second hand from a clock
int Microsteps = 16; //Stepsize (1 for full steps, 2 for half steps, 4 for quarter steps, etc)

//Providing parameters for motor control
A4988 stepper(spr, Dire, Step, MS1, MS2, MS3);

unsigned long loopStart;

void setup() {
  Serial.begin(9600);
  pinMode(Step, OUTPUT); //Step pin as output
  pinMode(Dire,  OUTPUT); //Direcction pin as output
  pinMode(Sleep,  OUTPUT); //Set Sleep OUTPUT Control button as output
  digitalWrite(Step, LOW); // Currently no stepper motor movement
  digitalWrite(Dire, LOW);
  
  // Set target motor RPM to and microstepping setting
  stepper.begin(RPM, Microsteps);
}

void loop() {
    loopStart = millis();

    digitalWrite(Sleep, HIGH); //A logic high allows normal operation of the A4988 by removing from sleep
    stepper.rotate(360);
    digitalWrite(Sleep, LOW);

    unsigned long elapsed = millis() - loopStart;
    unsigned long remaining = 60000UL > elapsed ? 60000UL - elapsed : 0;

    // Sleep in chunks (watchdog only allows up to ~8s)
    while (remaining > 0) {
      wdt_reset();  // prevent watchdog reset
      unsigned long sleepMs = remaining > 8000 ? 8000 : remaining;
      delay(sleepMs);
      remaining -= sleepMs;
    }
}
