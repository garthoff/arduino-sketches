/*  PS2Keyboard library example
 
 PS2Keyboard now requries both pins specified for begin()
 
 keyboard.begin(data_pin, irq_pin);
 
 Valid irq pins:
 Arduino:      2, 3
 Arduino Mega: 2, 3, 18, 19, 20, 21
 Teensy 1.0:   0, 1, 2, 3, 4, 6, 7, 16
 Teensy 2.0:   5, 6, 7, 8
 Teensy++ 1.0: 0, 1, 2, 3, 18, 19, 36, 37
 Teensy++ 2.0: 0, 1, 2, 3, 18, 19, 36, 37
 Sanguino:     2, 10, 11
 
 for more information you can read the original wiki in arduino.cc
 at http://www.arduino.cc/playground/Main/PS2Keyboard
 or http://www.pjrc.com/teensy/td_libs_PS2Keyboard.html
 
 Like the Original library and example this is under LGPL license.
 
 Modified by Cuninganreset@gmail.com on 2010-03-22
 Modified by Paul Stoffregen <paul@pjrc.com> June 2010
 */

/*
  LiquidCrystal Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

/*
  PIR PIN: 5v, DATA, GND
*/

// include the library code:
#include <LiquidCrystal.h>
#include <PS2Keyboard.h>

//initialize keyboard
const int DataPin = 7;
const int IRQpin =  2;
PS2Keyboard keyboard;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 6);

//PIR
const int pirPin = 8;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.print("System Online");

  // keyboard startup
  delay(1000);
  keyboard.begin(DataPin, IRQpin);
  Serial.begin(9600);
  Serial.println("Keyboard Test:");
  
  //Pin Modes
  pinMode(pirPin, INPUT);
  
}

int lcdCursor = 0;
int pirState = 0;
int motionDetect = 0;

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //  lcd.setCursor(0, 1);
  
  // print the number of seconds since reset:
  //  lcd.print(millis()/1000);

  if (keyboard.available()) {

    lcd.setCursor(lcdCursor,2);
    lcd.blink();

    // read the next key
    char c = keyboard.read();

    // check for some of the special keys
    if (c == PS2_ENTER) {
      Serial.println();
    } 
    else if (c == PS2_TAB) {
      Serial.print("[Tab]");
    } 
    else if (c == PS2_ESC) {
      Serial.print("[ESC]");
    } 
    else if (c == PS2_PAGEDOWN) {
      Serial.print("[PgDn]");
    } 
    else if (c == PS2_PAGEUP) {
      Serial.print("[PgUp]");
    } 
    else if (c == PS2_LEFTARROW) {
      Serial.print("[Left]");
    } 
    else if (c == PS2_RIGHTARROW) {
      Serial.print("[Right]");
    } 
    else if (c == PS2_UPARROW) {
      Serial.print("[Up]");
    } 
    else if (c == PS2_DOWNARROW) {
      Serial.print("[Down]");
    } 
    else if (c == PS2_DELETE) {
      Serial.print("[Del]");
    } 
    else {

      // otherwise, just print all normal characters
      Serial.print(c);
      lcd.print(c);
      lcdCursor++;
    }
  }
  
  motionDetect = digitalRead(pirPin);
  if (motionDetect == HIGH) {
    lcd.setCursor(0,3);
    lcd.print("Motion Detected");
    pirState = HIGH;
  } else {
    if (pirState == HIGH) {
      lcd.setCursor(0,3);
      lcd.print("Motion Ended");
      pirState = LOW;
    }
  }
}



