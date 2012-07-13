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
unsigned long record_id = 0;
int RedLED = 2;
int GreenLED = 3;

void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);

  // red RedLED indicates error
  digitalWrite(RedLED, HIGH);

  while(1);
}

void setup() {
  Serial.begin(9600); 
  Serial.println("DHTxx test!");

  Wire.begin();
  RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiRedLED
    //RTC.adjust(DateTime(__DATE__, __TIME__));
  }

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

  // create a new file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      logfile.print("This file starts at: ");
      DateTime now = RTC.now();
      logfile.print(now.unixtime()); // seconds since 1/1/1970
      logfile.print(", ");
      logfile.print(now.year(), DEC);
      logfile.print("/");
      logfile.print(now.month(), DEC);
      logfile.print("/");
      logfile.print(now.day(), DEC);
      logfile.print(" ");
      logfile.print(now.hour(), DEC);
      logfile.print(":");
      logfile.print(now.minute(), DEC);
      logfile.print(":");
      logfile.println(now.second(), DEC);
      logfile.println("This probably means that something went wrong with the previous file");
      logfile.println("ID, UNIXTIME, DATE, TIME, TEMPERATURE, WARNING FLAG");
      break;  // leave the loop!
    }
  }

  if (! logfile) {
    error("couldnt create file");
  }

  Serial.print("Logging to: ");
  Serial.println(filename);

  dht.begin();

  pinMode(RedLED, OUTPUT);
  pinMode(GreenLED, OUTPUT);
}

void loop () {
  DateTime now = RTC.now();

  if (now.minute() != previousMeasurementTime) {

    logfile.print(++record_id);
    logfile.print(", ");
    logfile.print(now.unixtime()); // seconds since 1/1/1970
    logfile.print(", ");
    logfile.print(now.year(), DEC);
    logfile.print("/");
    logfile.print(now.month(), DEC);
    logfile.print("/");
    logfile.print(now.day(), DEC);
    logfile.print(", ");
    logfile.print(now.hour(), DEC);
    logfile.print(":");
    logfile.print(now.minute(), DEC);
    logfile.print(":");
    logfile.print(now.second(), DEC);

    Serial.print(record_id);
    Serial.print("\t");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print('\t');

    float t = dht.readTemperature();

    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t)) {
      Serial.println("FaiRedLED to read from DHT");
    } 
    else {
      logfile.print(", "); 
      logfile.print(t);

      Serial.print("Temperature: "); 
      Serial.print(t);
      Serial.print(" *C");
    }

    if (t > 30) {
      digitalWrite(RedLED, HIGH);
      logfile.println(", VERY HIGH");
      Serial.println("\t WARNING");
    } 
    else {
      digitalWrite(RedLED, LOW);
      logfile.println();
      Serial.println();
    }

    digitalWrite(GreenLED, HIGH);
    logfile.flush();
    digitalWrite(GreenLED, LOW);

    previousMeasurementTime = now.minute();
  }
}








