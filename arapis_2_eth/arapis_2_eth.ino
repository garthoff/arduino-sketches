#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
const int chipSelect = 4; // Arduino Ethernet shield: pin 4
Sd2Card card;
File logfile;

//network configuration
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0x69, 0xB3 }; //physical mac address
byte ip[] = { 
  10, 10, 15, 71 }; // ip in lan

const int boardLed = 9;

#include "DHT.h"
#define DHTPIN 7
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

byte previousMeasurementTime = 0;
unsigned int record_id = 0;

void newSdFile() {
  char filename[] = "ZKN_00.CSV";
//  for (uint8_t i = 0; i < 100; i++) {
//    filename[6] = i/10 + '0';
//    filename[7] = i%10 + '0';
//    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE);
     logfile.println("Start"); 
//      logfile.print("This file starts at: ");
//      DateTime now = RTC.now();
//      logfile.print(now.unixtime()); // seconds since 1/1/1970
//      logfile.print(", ");
//      logfile.print(now.year(), DEC);
//      logfile.print("/");
//      logfile.print(now.month(), DEC);
//      logfile.print("/");
//      logfile.print(now.day(), DEC);
//      logfile.print(" ");
//      logfile.print(now.hour(), DEC);
//      logfile.print(":");
//      logfile.print(now.minute(), DEC);
//      logfile.print(":");
//      logfile.println(now.second(), DEC);
//      logfile.println("This probably means that something went wrong with the previous file");
//      logfile.println("ID, UNIXTIME, DATE, TIME, TEMPERATURE, WARNING FLAG");
//      break;  // leave the loop!
//    }
//  }
  
  if (! logfile) {
    Serial.println("couldnt create file");
  } else {
    Serial.print("Logging to: ");
    Serial.println(filename);
  }

}

void setup(){
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  dht.begin();

  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed!");
    return;
  } 
  else {
    Serial.println("initialization done.");
  }

  newSdFile();

  pinMode(boardLed, OUTPUT);

  /*  if (!volume.init(card)) {
   Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
   return;
   }*/
}
void loop(){

  digitalWrite(boardLed, LOW);

  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Failed to read from DHT");
  } 
  else {
    Serial.println(t);
  }

  if (t>30) {
    digitalWrite(boardLed, HIGH);
  }

  delay(1000);
}





