//#define arapstore "GNR"
//#define arapstore "PNX"
//#define arapstore "OVR"
//#define arapstore "TEI"
//#define arapstore "ZKN"
#define arapstore "BRX"

#include <SPI.h>
#include <Ethernet.h>

//#include <SD.h>
//#include <ICMPPing.h>
//SOCKET pingSocket = 0;
//char buffer [256];

//network configuration
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0x69, 0xB3 }; //physical mac address
//byte gateway[] = { 
//  192, 168, 8, 1 };
//byte subnet[] = {
//  255, 255, 255, 0 };

IPAddress ip(192,168,8,198); // ip in lan
IPAddress server(192,168,1,11); //kronos
byte server1[] = { 
  192, 168, 1, 11 };
EthernetClient client;

//String textmessage = String();
unsigned long notificationTime = 0;
const unsigned long timeBeetweenNotifications = 600000; //do not send another notification in the same hour
char* recipients[] = { 
  "<bill@seremetis.net>", "<tserem@gmail.com>", "<malatar@hol.gr>" };
const byte peopleToNotify = 3; //how many from the above list to notify, by order of precedence
int fridgeId;

void SendEMail(int recipientId) {
  if (client.connect(server, 25)) {
    Serial.println("connected");
  } 
  else {
    Serial.println("connection failed");
  }
  delay(5000);
  
  if (client.available()) {
    Serial.println("Sending email");
    client.print("HELO ");
    client.println(arapstore);
    client.println("MAIL FROM:<arapis3a@otenet.gr>");
    //      client.println("RCPT TO:<bill@seremetis.net>");
    client.print("RCPT TO:");
    client.println(recipients[recipientId]);
    client.println("DATA");
    client.println("From: <arapis3a@otenet.gr>");
    //      client.println("TO: <bill@seremetis.net>");
    client.print("TO: ");
    client.println(recipients[recipientId]);
    client.println("SUBJECT: Arduino automatic email");
    client.println();
    client.println(arapstore);
    client.println("Refridgerator Malfunction (nightly build)");
    
    //      client.println(textmessage);
    client.println(".");
    delay(200);
    client.println("QUIT");
    notificationTime = millis();
  }
  else {
    Serial.println("2.connection failed");
  }
}

boolean checkFridges() {
}

void setup()

{
  Ethernet.begin(mac, ip);
  Serial.begin(9600);
  Serial.println(Ethernet.localIP());
  delay(5000);

  for (fridgeId = 0; fridgeId < 6; fridgeId++) {
    pinMode(fridgeId+2, INPUT);
  }

//  Serial.println("connecting...");

  //  ICMPPing ping(pingSocket);
  //  ping(4, server1, buffer);
  //  Serial.println(buffer);
  //  delay(500);

  
}


void loop() {
  for (fridgeId = 0; fridgeId < 6; fridgeId++) {
//    Serial.println(digitalRead(fridgeId+2));
    if (digitalRead(fridgeId+2) == HIGH) {
//      Serial.println(fridgeId+1);
//      Serial.println(digitalRead(fridgeId+2));
//      Serial.println("execute");

      if((millis() - notificationTime > timeBeetweenNotifications) || (notificationTime == 0)) {

        //    Serial.println("in loop if 2");
        //      textmessage = arapstore;

        //      textmessage += fridgeId;
        for (int i = 0; i < peopleToNotify; i++) {
          SendEMail(i);
        }
      }
    }
  }
  delay(5000);


  //    ICMPPing ping(pingSocket);
  //    ping(4, server1, buffer);
  //    Serial.println(buffer);
  //    delay(1500);

  //  if (client.available()) {
  //     char c = client.read();
  //     Serial.print(c);
  //   }

  //   if (!client.connected()) {
  //     Serial.println();
  //     Serial.println("disconnecting.");
  //     client.stop();
  //     for(;;)
  //       ;
  //   }

}
