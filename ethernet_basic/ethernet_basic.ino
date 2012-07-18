#include <SPI.h>
#include <Ethernet.h>
//#include <SD.h>

//network configuration
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x69, 0xB3 }; //physical mac address
byte ip[] = { 10, 10, 15, 71 }; // ip in lan


String readString;

//Initialize Ethernet Server Library
EthernetServer server(80);

void setup() {
  //Start Ethernet and Server
  Ethernet.begin(mac, ip);
  server.begin();
  
  Serial.begin(9600);
  Serial.println("server LED test 1.0"); // so I can keep track of what is loaded
}

void loop(){

  // Create a client connection

  EthernetClient client = server.available();

  if (client) {

    while (client.connected()) {

      if (client.available()) {

        char c = client.read();

 

        //read char by char HTTP request

        if (readString.length() < 100) {

 

          //store characters to string 

          readString += c; 

          //Serial.print(c);

        } 

 

        //if HTTP request has ended

        if (c == '\n') {

 

          ///////////////

          Serial.println(readString); //print to serial monitor for debuging 

 

          client.println("HTTP/1.1 200 OK"); //send new page

          client.println("Content-Type: text/html");

          client.println();

 

          client.println("<HTML><HEAD>");
          client.println("<TITLE>Home Automation</TITLE>");
          client.println("</HEAD><BODY>");

          client.println("<H1>Home Automation</H1>");

          client.println("<hr /><br />");
          client.println("<a href=\"/?lighton\"\">Turn On Light</a>"); 
          client.println("<a href=\"/?lightoff\"\">Turn Off Light</a><br />");         

 

          client.println("</BODY></HTML>");

 

          delay(1);

          //stopping client

          client.stop();

 

          ///////////////////// control arduino pin

          if(readString.indexOf("?lighton") >0)//checks for on

          {

            digitalWrite(5, HIGH);    // set pin 4 high


          }

          else{

          if(readString.indexOf("?lightoff") >0)//checks for off

          {

            digitalWrite(5, LOW);    // set pin 4 low

          }

          }

          //clearing string for next read

          readString="";

 

        }

      }

    }

  }

}
