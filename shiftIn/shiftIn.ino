//define where your pins are
int latchPin = 8;
int dataPin = 9;
int clockPin = 7;

//Define variables to hold the data 
//for shift register.
//starting with a non-zero numbers can help
//troubleshoot
byte incoming = 254;
byte incoming2 = 254;

void setup() {
  //start serial
  Serial.begin(9600);

Serial.println(incoming, BIN);
Serial.println("start");
  //define pin modes
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT); 
  pinMode(dataPin, INPUT);

}

void loop() {

  //Pulse the latch pin:
  //set it to 1 to collect parallel data
  digitalWrite(latchPin,1);
  //set it to 1 to collect parallel data, wait
  delayMicroseconds(100);
  //set it to 0 to transmit data serially  
  digitalWrite(latchPin,0);

  //while the shift register is in serial mode
  //collect each shift register into a byte
  //the register attached to the chip comes in first 
  incoming = shiftIn(dataPin, clockPin, MSBFIRST);
  incoming2 = shiftIn(dataPin, clockPin, MSBFIRST);

  //Print out the results.
  //leading 0's at the top of the byte 
  //(7, 6, 5, etc) will be dropped before 
  //the first pin that has a high input
  //reading  
  Serial.println(incoming, BIN);
  Serial.println(incoming2, BIN);

//white space
Serial.println("-------------------");
//delay so all these print satements can keep up. 
delay(500);

}
