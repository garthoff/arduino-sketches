//#define _STORE "GNR"
//#define _STORE "PNX"
//#define _STORE "OVR"
//#define _STORE "TEI"
//#define _STORE "ZKN"
//#define _STORE "BRX"
#define _STORE "LAB"

//#define _SMTP_HELO "HELO KRONOS" //needs to be tested
#define _SMTP_HELO "HELO smtp.hol.gr\n"
#define _SMTP_FROM "MAIL FROM: <seremetis@hol.gr>\n"

#define _DEBUGMODE 1 //set to 1 for debug mode, it prints more stuff to the serial and increases sketch size
#define _SETMODE 1 //use this to set alarm thresholds
//#define _ENABLE_GSM 1 //set to 0 if you don't have a GSM shield
//#define _ENABLE_EMAIL 1  //set to 0 if you do not want to send email alerts

//#include <avr/pgmspace.h>  //we could use progmem to handle the strings without killing our RAM
#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial cell(2,3); //create a software serial port for the GSM module
#include "RTClib.h"
RTC_DS1307 RTC;

#include <SdFat.h>
SdFat sd;
SdFile myFile;

const int chipSelect = 4; //sd chipselect pin

byte lastMinute = 0;
byte alarmFlag;
unsigned long notificationTime = 0;
const unsigned long timeBeetweenNotifications = 3600000; //do not send another notification in the same hour
const char* emailRecipients[] = { 
  "<bill@seremetis.net>", "<bserem@gmail.com>", "<tserem@gmail.com>", "<malatar@hol.gr>" };
const  char* mobileRecipients[] = { "6997047042", "6993730101" };
const byte peopleToNotify = 1; //how many from the above list to notify, by order of precedence
String textmessage = String();
String datastring = String();
String convertionChar = String(4);
int recordId = 0;

//network configuration
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0x69, 0xB3 }; //physical mac address
byte ip[] = { 
  10, 10, 15, 71 }; // ip in lan
byte subnet[] = { 
  255, 255, 255, 0 };
byte gateway[] = { 
  10, 10, 15, 1 };
byte mydns[] = {
//  8, 8, 8, 8 }; //google dns
  208, 67, 222, 222 }; //opendns
byte smtp[] = { 
  //  192, 168, 1, 11 }; //kronos
  194, 30, 193, 52 };  //smtp.hol.gr

EthernetClient client;

// Data wire is plugged into port # on the Arduino
#define ONE_WIRE_BUS 7
#define TEMPERATURE_PRECISION 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
const byte NUMBER_OF_SENSORS = 3;
DeviceAddress thermos[NUMBER_OF_SENSORS];

/* MY DEVICES
once development has completed devices should be declared instead of searched at boot
0x28, 0x1C, 0xF6, 0x14, 0x04, 0x00, 0x00, 0xAA
0x28, 0x3C, 0xE7, 0x14, 0x04, 0x00, 0x00, 0xAD
*/


void setup() {
  //Start Services (libraries that is)
  Ethernet.begin(mac, ip, mydns, gateway, subnet);
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();

  //Initialize GSM Module
  cell.begin(9600);
//  delay(35000); //insert 35sec delay for the SIM card to connect to the network
  cell.print(F("AT+SBAND=4\r\n")); //Set GSM to Cosmote/Wind. Look here: http://roamtheworldcellphones.com/gsmgreece
  delay(500);
  //GSM should be fine by now

  if (! RTC.isrunning()) {
    Serial.print(F("RTC is NOT running!\n"));
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  // Start up the DallasTemperature library
  sensors.begin();

#if _DEBUGMODE
  // so I can keep track of what is loaded
//  Serial.print("email with hol of temp sensors\nlocal ip: ");
  Serial.print(Ethernet.localIP());

  // locate devices on the bus
  Serial.print(F("\r\nFound "));
//  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.print(F(" devices.\nParasite: "));
  // report parasite power requirements
//  Serial.print("Parasite: "); 
  if (sensors.isParasitePowerMode()) Serial.print(F("ON\r\n"));
  else Serial.print(F("OFF\r\n"));
#endif

  // assign address manually.  the addresses below will beed to be changed
  // to valid device addresses on your bus.  device address can be retrieved
  // by using either oneWire.search(deviceAddress) or individually via
  // sensors.getAddress(deviceAddress, index)
  //t1 = { 0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF0 };
  //t2   = { 0x28, 0x3F, 0x1C, 0x31, 0x2, 0x0, 0x0, 0x2 };

  // search for devices on the bus and assign based on an index.  ideally,
  // you would do this to initially discover addresses on the bus and then 
  // use those addresses and manually assign them (see above) once you know 
  // the devices on your bus (and assuming they don't change).
  // 
  // method 1: by index
  //  if (!sensors.getAddress(t1, 0)) Serial.println("Unable to find address for Device 0"); 
  //  if (!sensors.getAddress(t2, 1)) Serial.println("Unable to find address for Device 1"); 

  // method 2: search()
  // search() looks for the next device. Returns 1 if a new address has been
  // returned. A zero might mean that the bus is shorted, there are no devices, 
  // or you have already retrieved all of them.  It might be a good idea to 
  // check the CRC to make sure you didn't get garbage.  The order is 
  // deterministic. You will always get the same devices in the same order
  //
  // Must be called before search()
  oneWire.reset_search();
  for (int i = 0; i < NUMBER_OF_SENSORS; i++) {
    // assigns the first address found to t1
    if (!oneWire.search(thermos[i])) {
      Serial.print(F("err")); //found an error
      Serial.print(i);
    }
    Serial.print(F("\nAdr:"));
    printAddress(thermos[i]);

    // set the resolution to 9 bit
    sensors.setResolution(thermos[i], TEMPERATURE_PRECISION);

    Serial.print(F("\nRes:"));
    Serial.print(sensors.getResolution(thermos[i]), DEC); 
  }

#if _SETMODE
  sensors.setLowAlarmTemp(thermos[0], 25);
  sensors.setHighAlarmTemp(thermos[0], 31);
  sensors.setLowAlarmTemp(thermos[1], 25);
  sensors.setHighAlarmTemp(thermos[1], 31);
  sensors.setLowAlarmTemp(thermos[2], 25);
  sensors.setHighAlarmTemp(thermos[2], 31);
#endif

//SendSMS(0); //just for testing

}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
//    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

/* 
 // function to print the temperature for a device
 void printTemperature(DeviceAddress deviceAddress)
 {
 float tempC = sensors.getTempC(deviceAddress);
 Serial.print("Temp C: ");
 Serial.print(tempC);
 }
 
 // function to print a device's resolution
 void printResolution(DeviceAddress deviceAddress)
 {
 Serial.print("Resolution: ");
 Serial.print(sensors.getResolution(deviceAddress));
 Serial.println();    
 }
 
 // main function to print information about a device
 void printData(DeviceAddress deviceAddress)
 {
 Serial.print("Device Address: ");
 printAddress(deviceAddress);
 Serial.print(" ");
 printTemperature(deviceAddress);
 Serial.println();
 }
 */

float myGetTemperature(DeviceAddress deviceAddress)
{
  return sensors.getTempC(deviceAddress);
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  Serial.print("\nAdr:");
  printAddress(deviceAddress);
   Serial.print(myGetTemperature(deviceAddress));
/*   int temp1 = myGetTemperature(deviceAddress);
   datastring = String(temp1);
   Serial.print(temp1);
*/
}

void checkAlarm(DeviceAddress deviceAddress)
{
  if (sensors.hasAlarm(deviceAddress))
  {
    Serial.print(F("\nALARM: "));
    printData(deviceAddress);
    alarmFlag = 1;
  }
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

#if _DEBUGMODE
void printWiznetBuffer() {
  char c = client.read();
  for (int i=0; i<1000; i++){
    c = client.read();
    Serial.print(c);
  }
}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void dummyMail(int recipientId) {
  Serial.print(emailRecipients[recipientId]);
  Serial.print("\r\n");
  notificationTime = millis();
}
#endif

void SendEmail(int recipientId) {
  boolean connectionSuccessfull = false;
  boolean mailSent = false;
  while (!mailSent) {
    if (client.connect(smtp, 25)) {
      Serial.print(F("conOk\n"));
      connectionSuccessfull = true;
    } 
    else {
      Serial.print(F("conFail\n"));
    }
    delay(2000);
    if (client.available() && connectionSuccessfull) {
//      Serial.print("Sending email\r\n");
      client.print(F(_SMTP_HELO));
      delay(50);
      client.print(F(_SMTP_FROM));
      delay(50);
      client.print(F("RCPT TO:"));
      client.print(emailRecipients[recipientId]);
      delay(50);
      client.print(F("\nDATA\n"));
      delay(50);
      client.print(F("From: <seremetis@hol.gr>\nTO: "));
//      client.print("TO: ");
      client.print(emailRecipients[recipientId]);
      client.print(F("\nSUBJECT: 3A\n\n"));
      client.print(F(_STORE));
      client.print(F("\nTEST MAIL\n"));
      //client.print(textmessage);
      client.print(F(".\n"));
      delay(100);
      client.print(F("QUIT\n"));
      notificationTime = millis();
#if _DEBUGMODE
      printWiznetBuffer();
#endif
//      Serial.print("ok\r\n");
      mailSent = true;
      client.stop();
    }
    else {
      Serial.print(F("conFail2\n"));
    }
  }
}

void SendSMS(int phoneId) {
  //  StartSMS
  cell.print(F("AT+CMGF=1\r\n")); // set SMS mode to text
  cell.print(F("AT+CMGS=\""));
  cell.print(mobileRecipients[phoneId]);
  cell.print(F("\"\r\n"));
  delay(500);

  //SendSMS main body
  cell.print(textmessage);

  //  EndSMS
  delay(500);
  cell.write(26);
  delay(500);
  Serial.print(textmessage);
  Serial.print(F("\nsent to: "));
  Serial.print(mobileRecipients[phoneId]);

  notificationTime = millis();
}

void loop(){
  datastring = "";
  DateTime now = RTC.now();
//  if (now.minute() != lastMinute) {
  if (1){

//    Serial.print("Requesting temperatures...");
    // call sensors.requestTemperatures() to issue a global temperature 
    // request to all devices on the bus
    sensors.requestTemperatures();

//    Serial.print("\n");
//    Serial.print(now.hour());
//    printDigits(now.minute());
//    printDigits(now.second());
//    Serial.print(" ");
//    Serial.print(now.day());
//    Serial.print("/");
//    Serial.print(now.month());
//    Serial.print("/");
//    Serial.print(now.year()); 
//    Serial.print("\r\n"); 
    lastMinute = now.minute();

    for (int i = 0; i < 3; i++){
      printData(thermos[i]);
      checkAlarm(thermos[i]);
    }

    if((millis() - notificationTime > timeBeetweenNotifications)) {
      notificationTime = 0;
      alarmFlag = 0;
    }

    //send emails at first
    if ((notificationTime == 0) && alarmFlag) {
      for (int i = 0; i < peopleToNotify; i++) {
//        SendEmail(i);
#if _DEBUGMODE
        dummyMail(i);
#endif
      }
      //send sms after emails, because they need more time to be completed
      for (int i=0; i<peopleToNotify; i++) {
//           SendSMS(i);
      }
    } 

//#if _DEBUGMODE
    Serial.print(F("\n\n"));
    Serial.print(freeRam());
//#endif

    logToSD();
    recordId++;
  }
}

void logToSD() {
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) sd.initErrorHalt();

  // open the file for write at end like the Native SD library
  if (!myFile.open("test.txt", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("open fail");
  }

  // if the file opened okay, write to it:
  Serial.print("Writing to test.txt...\r\n");
  myFile.print(recordId);

  // close the file:
  myFile.close();
}
