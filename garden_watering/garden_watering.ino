/*

 This sketch opens and closes some electrovalves that water my garden.
 Valves open one after the other.
 A delay between them is needed to make sure that the water tank doesn't empty.
 Each valve can stay open for a specified amount of time.
 1 minute = 60000 milliseconds

 Read times from SD card.
 Times are expressed with 2 digits in minutes. Each time in a different valve.
 First (0) time will be assigned to first (0) valve etc...
 
 The SD circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 10 - can be changed below

*/
 
#include <SD.h>
#include <avr/sleep.h>  //powerdown library

const int chipSelect = 10; //pin assinged to CS of SD card

File myFile;
const int valves[8] = { 0,1,2,3,4,5,6,7 }; //pins of valves on the Arduino

char _times[8][3]; //stores times read from SD
int times[8];  //stores times as minutes, after atoi()
int valve;  //used for valve increment
int i;


void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT);
   
  for (i = 0; i < 8; i++) {
    pinMode(valves[i],OUTPUT);
  }
   
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
  // open the file for reading:
  myFile = SD.open("garden.txt");
  if (myFile) {
    Serial.println("garden.txt:");
    
    //get times    
    for (valve = 0; valve < 8; valve++) {
      for (i = 0; i < 3; i++) { //we read two digits, third is left blank for atoi to work (I've read it, I did it, it works)
        _times[valve][i] = myFile.read();
      }
      //Serial.println(_times[valve]);
      times[valve] = atoi(_times[valve]);
      //Serial.println(times[valve]);
    }    
        
    // close the file:
    myFile.close();
    //Serial.println("file closed");
  } else {
  	// if the file didn't open, print an error:
    Serial.println("error opening garden.txt");
  }
}

void water(int valve, int minutes) {
  unsigned long millitime = minutes * 60000;
  digitalWrite(valve, HIGH);
  delay(millitime);
  digitalWrite(valve, LOW);
  delay(60000); //wait a minute, this may need to be increased for real life
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
  for (i = 0; i < 8; i++) {
    water(valves[i], times[i]);
  }
  
  sleepNow(); //powerdown
}


