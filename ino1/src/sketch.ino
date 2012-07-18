#include <Arduino.h>
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
#define LED 9

void setup() {                
  // initialize the digital pin as an output.
  // Pin LED has an LED connected on most Arduino boards:
  pinMode(LED, OUTPUT);     
  Serial.begin(9600);
}

void loop() {
  digitalWrite(LED, HIGH);   // set the LED on
  delay(1000);              // wait for a second
  digitalWrite(LED, LOW);    // set the LED off
  delay(500);              // wait for a second
  Serial.println("dude");
}
