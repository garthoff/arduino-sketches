char* mobileRecipients[] = { "6997047042", "6973317373" };
const byte peopleToNotify = 1; //how many from the above list to notify, by order of precedence
String textmessage = String();
#include <SoftwareSerial.h>
SoftwareSerial cell(2,3);

unsigned long notificationTime = 0;
const unsigned long timeBeetweenNotifications = 3600000; //do not send another notification in the same hour
int fridgeId;

//#define arapstore "GNR"
//#define arapstore "PNX"
//#define arapstore "OVR"
//#define arapstore "TEI"
//#define arapstore "ZKN"
#define arapstore "BRX"

void SendSMS(int phoneId) {
  //  StartSMS
  cell.println("AT+CMGF=1"); // set SMS mode to text
  cell.print("AT+CMGS=\"");
  cell.print(mobileRecipients[phoneId]);
  cell.println("\"");
  delay(500);

  //SendSMS main body
  cell.print(textmessage);

  //  EndSMS
  delay(500);
  cell.write(26);
  delay(500);
  Serial.println(textmessage);
  Serial.print("message sent to: ");
  Serial.println(mobileRecipients[phoneId]);

  notificationTime = millis();
}

void setup() {
  cell.begin(9600);
  Serial.begin(9600);
  Serial.println("Starting SM5100B Communication...");
  delay(35000); //insert 35sec delay for the SIM card to connect to the network
  cell.println("AT+SBAND=4"); //Set GSM to Cosmote/Wind. Look here: http://roamtheworldcellphones.com/gsmgreece
  delay(500);
}

void loop(){
  if((millis() - notificationTime > timeBeetweenNotifications) || (notificationTime == 0)) {
    textmessage = arapstore;
    textmessage += fridgeId;
    for (int i = 0; i < peopleToNotify; i++) {
//      SendSMS(i);
    }
  }
}




