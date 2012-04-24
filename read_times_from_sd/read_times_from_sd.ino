/*
 Read times from SD card
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 - can be changed below
 
 */
 
#include <SD.h>
const int chipSelect = 4; //pin assinged to CS of SD card

File myFile;

char _times[8][5];
int times[8];
int valve;
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
   
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
  // re-open the file for reading:
  myFile = SD.open("kipos.txt");
  if (myFile) {
    Serial.println("kipos.txt:");
    
    //get times
    
    for (valve = 0; valve < 8; valve++) {
      for (i = 0; i < 5; i++) {
        _times[valve][i] = myFile.read();
      }
      Serial.println(_times[valve]);
      times[valve] = atoi(_times[valve]);
      Serial.println(times[valve]);
    }    
        
    // close the file:
    myFile.close();
    Serial.println("file closed");
  } else {
  	// if the file didn't open, print an error:
    Serial.println("error opening kipos.txt");
  }
  
    //Serial.println(times);
}

void loop()
{
  //do nothing
}


