/*

 This sketch opens and closes some electrovalves that water my garden.
 Valves open one after the other.
 A delay between them is needed to make sure that the water tank doesn't empty.
 Each valve can stay open for a specified amount of time.
 1 minute = 60000 milliseconds

*/
 
#include <EEPROM.h>  //EEPROM read/write library
#include <avr/sleep.h>  //powerdown library


const int valves[7] = { 5,6,7,8,9,10,11 }; //pins of valves on the Arduino

int times[7] = { 1,90,150,120,120,120,60 };  //stores times as seconds
//int valve;  //used for valve increment
int i;  //counter, used to increment from valve to valve
int tankLed = 13; //light a led when the water tank refills

void setup()
{
  Serial.begin(9600);

  for (i = 0; i < 7; i++) {
    pinMode(valves[i],OUTPUT);
    pinMode(tankLed, OUTPUT);
  }
  
  //emulate something like a boot process, for fun  
  digitalWrite(tankLed, HIGH);
  delay(15000);
  digitalWrite(tankLed, LOW);

  
  int address = 0;
  byte value;
  for (address = 0; address < 61; address++) {
    value = EEPROM.read(address);
    Serial.print(address);
    Serial.print("\t");
    Serial.print(value, DEC);
    Serial.println();
  }

  
}

void water(int valve, int seconds) {
  Serial.print("Valve: \t");
  Serial.print(i+1); //the physical valve
  Serial.print("\tArduino Pin: \t");
  Serial.print(valve); //the valve pin
  Serial.print("\tTime in minutes: \t");
  Serial.println(seconds);  //time
  
  unsigned long millitime = seconds * 1000;
  
  //open the valve and water a specific garden area
  digitalWrite(valve, HIGH);
  delay(millitime);
  digitalWrite(valve, LOW);
  
  //allow some time for the water tank to refill
  digitalWrite(tankLed, HIGH);
  delay(60000); //wait a minute, this may need to be increased for real life
  digitalWrite(tankLed, LOW);
}

void sleepNow() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  ADCSRA &= ~(1 << ADEN);    // Disable ADC
  PRR = 0xFF;   // Power down functions
  sleep_mode();
}

void loop()
{
  for (i = 0; i < 7; i++) {
    water(valves[i], times[i]);
  }
  
  sleepNow(); //powerdown
}


