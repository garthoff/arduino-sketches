/*

 This sketch opens and closes some electrovalves that water my garden.
 Valves open one after the other.
 A delay between them is needed to make sure that the water tank doesn't empty.
 Each valve can stay open for a specified amount of time.


 v2.0beta - 2012/06/19
 Modification to work with seconds instead of minutes. It works now.
 Display times on serial port on startup.
 Removed any time multiplications, using a custom delay function that "works" with seconds repeatedly in order to achieve the required delay.
 Increased startup time to 30 seconds. So as to re-program before the program starts to garden.
 Removed EEPROM library, for now.
 
 v1.5 - 2012/06/17
 Modified to work with seconds: IT WAS A DISASTER... v2 CAME AROUND
 
 v1.0 - 2012/06
 Initial version. Works with minutes.
*/

#include <avr/sleep.h>  //powerdown library

const int valves[7] = { 
  5,6,7,8,9,10,11 }; //pins of valves on the Arduino

int water_times[7] = { 
//  60,90,150,120,120,120,60 };  //stores times as seconds
  0,30,150,0,120,120,60 };

int i;  //counter, used to increment from valve to valve
int tankLed = 13; //light a led when the water tank refills

void delaySeconds(unsigned long _seconds){
  while (_seconds > 0){
    delay(1000);
    _seconds--;
  }
}

void water(int valve, int watering_time_in_seconds) {
  Serial.print("Valve: \t");
  Serial.print(i+1); //the physical valve
  Serial.print("\tArduino Pin: \t");
  Serial.print(valve); //the valve pin
  Serial.print("\tTime in Seconds: \t");
  Serial.println(watering_time_in_seconds);  //time

  //open the valve and water a specific garden area
  digitalWrite(valve, HIGH);
  delaySeconds(watering_time_in_seconds);
  digitalWrite(valve, LOW);

  //allow some time for the water tank to refill
  digitalWrite(tankLed, HIGH);
  Serial.println("Filling tank");
  delaySeconds(60); //wait a minute, this may need to be increased for real life
  digitalWrite(tankLed, LOW);
  Serial.println("Tank filling stopped.");
}

void sleepNow() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  ADCSRA &= ~(1 << ADEN);    // Disable ADC
  PRR = 0xFF;   // Power down functions
  sleep_mode();
}

void setup()
{
  Serial.begin(9600);

  for (i = 0; i < 7; i++) {
    pinMode(valves[i],OUTPUT);
    Serial.print("Valve: \t");
    Serial.print(i+1); //the physical valve
    Serial.print("\tTime in seconds: \t");
    Serial.println(water_times[i]);  //time
  }

  pinMode(tankLed, OUTPUT);

  //emulate something like a boot process, for fun  
  digitalWrite(tankLed, HIGH);
  Serial.println("Starting in 30sec");
  delaySeconds(30);
  digitalWrite(tankLed, LOW);
  Serial.println("Started");

}

void loop()
{
  for (i = 0; i < 7; i++) {
    water(valves[i], water_times[i]);
  }

  sleepNow(); //powerdown
}





