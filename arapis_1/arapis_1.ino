#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

#include "DHT.h"
#define DHTPIN 8
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#include <SD.h>
const int chipSelect = 10; //pin assinged to CS of SD card

File logfile;

int previousMeasurementTime = -1;
unsigned int record_id = 0;

void setup() {
  Serial.begin(9600); 

  Wire.begin();
  RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  pinMode(chipSelect, OUTPUT);

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // create a new file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }

  if (! logfile) {
    Serial.println("couldnt create file");
  }

  Serial.print("Logging to: ");
  Serial.println(filename);

  Serial.println("DHTxx test!");
  dht.begin();

}

void loop () {
  if (record_id == 0) {
    logfile.println("ID, DATE, TIME, TEMPERATURE, FLAG");
  }

  DateTime now = RTC.now();

  if (now.minute() != previousMeasurementTime) {

    float temperature = dht.readTemperature();

    String row = "";
    row = row + record_id + ", " + now.year() + "/" + now.month() + "/" + now.day() + ", " + now.hour() + ":" + now.minute() + ":" + now.second() + ", ";
    logfile.print(row);

    Serial.print(row);


    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(temperature)) {
      Serial.println("Failed to read from DHT");
    } 
    else {
      logfile.print(temperature);
      Serial.println(temperature);
    }

    if (temperature > 30) {
      logfile.println(", VERY HIGH");
    } 
    else {
      logfile.println();

    }

    logfile.flush();
    
    record_id++;

    previousMeasurementTime = now.minute();
  }
}

