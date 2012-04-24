/*
  Countdown from 9 to 0 on a Seven Segment Display
  
  aaaa
  f  b
  fggb
  e  c
  e  c
  dddd
  
*/

int segG = 13;
int segF = 12;
int segE = 11;
int segD = 10;
int segC = 9;
int segB = 8;
int segA = 7;

//Arduino Pinout: 7,8,9,10,11,12,13

//DEC numbers expressed as individual segments (http://en.wikipedia.org/wiki/Seven-segment_display)
//seven_seg_digits[digitToPrint][segment]
byte seven_seg_digits[37][7] = {
  { 1,1,1,1,1,1,0 },  // = 0
  { 0,1,1,0,0,0,0 },  // = 1
  { 1,1,0,1,1,0,1 },  // = 2
  { 1,1,1,1,0,0,1 },  // = 3
  { 0,1,1,0,0,1,1 },  // = 4
  { 1,0,1,1,0,1,1 },  // = 5
  { 1,0,1,1,1,1,1 },  // = 6
  { 1,1,1,0,0,0,0 },  // = 7
  { 1,1,1,1,1,1,1 },  // = 8
  { 1,1,1,1,0,1,1 },  // = 9
  { 1,1,1,0,1,1,1 },  // = A 10
  { 0,0,1,1,1,1,1 },  // = b 11
  { 1,0,0,1,1,1,0 },  // = C 12
  { 0,1,1,1,1,0,1 },  // = d 13
  { 1,0,0,1,1,1,1 },  // = E 14
  { 1,0,0,0,1,1,1 },  // = F 15
  { 1,0,1,1,1,1,0 },  // = G 16
  { 0,1,1,0,1,1,1 },  // = H 17
  { 0,0,0,0,1,1,0 },  // = I 18
  { 0,1,1,1,1,0,0 },  // = J 19
  { 0,0,0,0,1,1,1 },  // = K 20
  { 0,0,0,1,1,1,0 },  // = L 21
  { 1,0,1,0,1,0,1 },  // = M 22
  { 0,0,1,0,1,0,1 },  // = n 23
  { 0,0,1,1,1,0,1 },  // = o 24
  { 1,1,0,0,1,1,1 },  // = P 25
  { 1,1,1,1,0,0,1 },  // = q 26
  { 0,0,1,1,1,0,1 },  // = r 27
  { 0,0,1,1,0,1,1 },  // = s 28
  { 0,0,0,1,1,1,1 },  // = t 29
  { 0,0,1,1,1,0,0 },  // = u 30
  { 0,1,1,1,1,1,0 },  // = V 31
  { 0,1,0,1,0,1,1 },  // = W 32
  { 1,0,0,1,0,0,1 },  // = X 33
  { 0,1,1,1,0,1,1 },  // = Y 34
  { 0,1,0,0,1,0,1 },  // = Z 35
  { 0,0,0,0,0,0,0 }   // blank 36
};

void setup() {                
  // initialize the digital pins as outputs.
  pinMode(segA, OUTPUT);     
  pinMode(segB, OUTPUT);     
  pinMode(segC, OUTPUT);     
  pinMode(segD, OUTPUT);     
  pinMode(segE, OUTPUT);     
  pinMode(segF, OUTPUT);     
  pinMode(segG, OUTPUT);     
  //The following line allows "delay" to work correctly under linux (https://wiki.archlinux.org/index.php/Arduino#delay.28.29_function_doesn.27t_work)
  Serial.begin(9600);
}

//In our wiring the segments are using pins 7 to 13.
//Pins are in order, this is important for the following code to work.
void sevSegWrite(byte digitToPrint) {
  byte pin = segA;
  for(byte segment = 0; segment < 7; ++segment) {
    digitalWrite(pin,seven_seg_digits[digitToPrint][segment]);
    ++pin;
  }
}

void loop() {
  for (byte digit = 1; digit < 37; digit++) {
    sevSegWrite(digit-1);
    delay(1000);
  }
/*
  sevSegWrite(16);
  delay(1000);
  sevSegWrite(14);
  delay(1000);
  sevSegWrite(17);
  delay(1000);
  sevSegWrite(18);
  delay(200);
  sevSegWrite(17);
  delay(1000);
  sevSegWrite(0);
  delay(1000);
  sevSegWrite(18);
  delay(5000);
 */
}
