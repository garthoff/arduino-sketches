//#define arapstore "GNR"
//#define arapstore "PNX"
//#define arapstore "OVR"
//#define arapstore "TEI"
//#define arapstore "ZKN"
#define arapstore "BRX"

#include <SPI.h>
#include <Ethernet.h>
//#include <SD.h>

//network configuration
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0x69, 0xB3 }; //physical mac address
byte gateway[] = { 
  192, 168, 8, 1 };
byte subnet[] = {
  255, 255, 255, 0 };

IPAddress ip(192,168,8,198); // ip in lan
IPAddress server(192,168,1,11); //kronos
EthernetClient client;

String textmessage = String();
unsigned long notificationTime = 0;
const unsigned long timeBeetweenNotifications = 3600000; //do not send another notification in the same hour
const byte peopleToNotify = 1; //how many from the above list to notify, by order of precedence
int fridgeId;

void SendEMail() {
  if (client.available()) {
    if(0) {
      Serial.println("Sending email");
      client.println("HELO BRX");
      client.println("MAIL FROM:<arapis3a@otenet.gr>");
      client.println("RCPT TO:<bill@seremetis.net>");
      client.println("DATA");
      client.println("From: <arapis3a@otenet.gr>");
      client.println("TO: <bill@seremetis.net>");
      client.println("SUBJECT: BRX: Arduino automatic email");
      client.println();
      client.println("body-ln1");
      client.println("body-ln2");
//      client.println(textmessage);
      client.println(".");
    } 
  }
  else {
    Serial.println("connection failed");
  }
}

boolean checkFridges() {
}

void setup()
{
  Ethernet.begin(mac, ip, gateway, subnet);
  Serial.begin(9600);

  delay(1000);

  Serial.println("connecting...");

  if (client.connect(server, 25)) {
    Serial.println("connected");
  } 
  else {
    Serial.println("connection failed");
  }
}

void loop() {
  if (checkFridges()) {
  if((millis() - notificationTime > timeBeetweenNotifications) || (notificationTime == 0)) {
    textmessage = arapstore;
    textmessage += fridgeId;
    for (int i = 0; i < peopleToNotify; i++) {
      SendEMail();
    }
  }
}
}
