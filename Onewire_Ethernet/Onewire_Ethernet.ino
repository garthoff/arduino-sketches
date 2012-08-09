#include <SPI.h>
#include <Ethernet.h>
//#include <SD.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

byte lastMinute = 0;

//network configuration
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0x69, 0xB3 }; //physical mac address
byte ip[] = { 
  10, 10, 15, 71 }; // ip in lan
byte subnet[] = { 
  255, 255, 255, 0 };
byte gateway[] = { 
  10, 10, 15, 1 };
//byte mydns[] = { 8, 8, 8, 8 }; //google dns
byte mydns[] = { 
  208, 67, 222, 222 }; //opendns

//Initialize Ethernet Server Library
EthernetServer server(80);

// Data wire is plugged into port 4 on the Arduino
#define ONE_WIRE_BUS 3
#define TEMPERATURE_PRECISION 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
//DeviceAddress t1, t2, t3;
DeviceAddress thermos[3];
const int NUMBER_OF_SENSORS = 3;

void setup() {
  //Start Ethernet and Server
  Ethernet.begin(mac, ip, mydns, gateway, subnet);

  Serial.begin(9600);
  Serial.println("temps on the web"); // so I can keep track of what is loaded

  Serial.println("Dallas Temperature IC Control Library Demo");

  Wire.begin();
  RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  // Start up the library
  sensors.begin();

  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

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
      Serial.print("unable sens");
      Serial.println(i);
    }
    Serial.print("Device Address: ");
    printAddress(thermos[i]);
    Serial.println();

    // set the resolution to 9 bit
    sensors.setResolution(thermos[i], TEMPERATURE_PRECISION);

    Serial.print("Device Resolution: ");
    Serial.print(sensors.getResolution(thermos[i]), DEC); 
    Serial.println();
  }

  sensors.setLowAlarmTemp(thermos[0], 25);
  sensors.setHighAlarmTemp(thermos[0], 30);
  sensors.setLowAlarmTemp(thermos[1], 25);
  sensors.setHighAlarmTemp(thermos[1], 30);
  sensors.setLowAlarmTemp(thermos[2], 28);
  sensors.setHighAlarmTemp(thermos[2], 30);
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
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

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  Serial.println(myGetTemperature(deviceAddress));
//  Serial.println();
}

void checkAlarm(DeviceAddress deviceAddress)
{
  if (sensors.hasAlarm(deviceAddress))
  {
    Serial.print("ALARM: ");
    printData(deviceAddress);
  }
}

void loop(){
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  //  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  //  Serial.println("DONE");

  // print the device information
  //  printData(t1);
  //  printData(t2);

  for (int i = 0; i < 3; i++){
    checkAlarm(thermos[i]);
  }

  DateTime now = RTC.now();

  if (now.minute() != lastMinute) {
    Serial.print(now.hour());
    printDigits(now.minute());
    printDigits(now.second());
    Serial.print(" ");
    Serial.print(now.day());
    Serial.print(" ");
    Serial.print(now.month());
    Serial.print(" ");
    Serial.print(now.year()); 
    Serial.println(); 
    lastMinute = now.minute();
  }

  EthernetClient client = server.available();

  if (client) {

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Content-Type: text/html");
          client.println("<HTML><HEAD><TITLE>Home Automation</TITLE></HEAD><BODY>");
          client.println("<H1>Home Automation</H1>");

          client.print(now.hour());
          client.print(now.minute());
          client.print(now.second());
          client.print(" ");
          client.print(now.day());
          client.print(" ");
          client.print(now.month());
          client.print(" ");
          client.print(now.year()); 
          client.println(); 

          for (int i=0; i<3; i++){
            client.println("<hr /><br />");
            float tempC = myGetTemperature(thermos[i]);
            client.println(tempC); 
            client.println(" celsius");
          }
          client.println("</BODY></HTML>");

          delay(1);

          //stopping client
          client.stop();
       }
      }
    }
  }
}
