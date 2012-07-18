#include <SPI.h>
#include <Ethernet.h>
//#include <SD.h>

//network configuration
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0x69, 0xB3 }; //physical mac address
IPAddress ip(192,168,8,198); // ip in lan
IPAddress server(192,168,1,11); //kronos
EthernetClient client;

void setup()
{
  Ethernet.begin(mac, ip);
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
      client.println("This is the body.");
      client.println("This is another line of the body.");
      client.println(".");
    } 
  }
  else {
    Serial.println("connection failed");
  }
}

