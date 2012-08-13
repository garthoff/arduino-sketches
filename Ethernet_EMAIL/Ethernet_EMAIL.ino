//#define _STORE "GNR"
//#define _STORE "PNX"
//#define _STORE "OVR"
//#define _STORE "TEI"
//#define _STORE "ZKN"
//#define _STORE "BRX"
#define _STORE "LAB"
#include <SPI.h>
#include <Ethernet.h>

String readString = String(100);

//#include <SD.h>
//#include <ICMPPing.h>
//SOCKET pingSocket = 0;
//char buffer [256];

//network configuration
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

//IPAddress server( 194, 30, 193, 52); //kronos
byte server1[] = { 
  //  192, 168, 1, 11 }; //kronos
  194, 30, 193, 52 };  //smtp.hol.gr
EthernetClient client;

//String textmessage = String();
unsigned long notificationTime = 0;
const unsigned long timeBeetweenNotifications = 600000; //do not send another notification in the same hour
char* recipients[] = { 
  "<bill@seremetis.net>", "<tserem@gmail.com>", "<malatar@hol.gr>" };
const byte peopleToNotify = 3; //how many from the above list to notify, by order of precedence
int fridgeId;

void printWiznetBuffer() {
  char c = client.read();
  for (int i=0; i<100; i++){
    c = client.read();
    Serial.print(c);
  }
}

void SendEmail(int recipientId) {
  if (client.connect(server1, 25)) {
    Serial.println("connected");
  } 
  else {
    Serial.println("connection failed");
  }
  delay(5000);

  if (client.available()) {
    Serial.println("Sending email");
    //    client.print("HELO ");
    //    client.println(_STORE);
    client.println("HELO smtp.hol.gr");
    printWiznetBuffer();


    delay(100);
    client.println("MAIL FROM:<seremetis@hol.gr>");
    printWiznetBuffer();

    delay(100);
    //      client.println("RCPT TO:<bill@seremetis.net>");
    client.print("RCPT TO:");
    client.println(recipients[recipientId]);
    printWiznetBuffer();

    delay(100);
    client.println("DATA");

    delay(100);
    //    client.println("From: <seremetis@hol.gr>");
    //    //      client.println("TO: <bill@seremetis.net>");
    //    client.print("TO: ");
    //    client.println(recipients[recipientId]);
    //    client.println("SUBJECT: Arduino automatic email");
    //    client.println();
    client.println(_STORE);
    client.println("TEST MAIL");

    //      client.println(textmessage);
    client.println(".");
    printWiznetBuffer();
    Serial.println("done");

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
  Ethernet.begin(mac, ip, mydns, gateway, subnet);
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

int flag = 0;
void loop() {
  //  for (fridgeId = 0; fridgeId < 6; fridgeId++) {
  ////    Serial.println(digitalRead(fridgeId+2));
  //    if (digitalRead(fridgeId+2) == HIGH) {
  ////      Serial.println(fridgeId+1);
  ////      Serial.println(digitalRead(fridgeId+2));
  ////      Serial.println("execute");
  //
if((millis() - notificationTime > timeBeetweenNotifications) || (notificationTime == 0)) {

  //        //    Serial.println("in loop if 2");
  //        //      textmessage = _STORE;
  //
  //        //      textmessage += fridgeId;
for (int i = 0; i < peopleToNotify; i++) {
SendEmail(i);
}
}
  //    }
  //  }

  if (flag == 0) {
    SendEmail(0);
    flag = 1;
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

