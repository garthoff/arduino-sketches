#include <avr/sleep.h>  //powerdown library

#define PIN_SCE   A1
#define PIN_RESET A0
#define PIN_DC    A2
#define PIN_SDIN  A3
#define PIN_SCLK  A4

#define LCD_C     LOW
#define LCD_D     HIGH
#define LCD_CMD   0

#define LCD_X     84
#define LCD_Y     48

int a = 0;

static const byte ASCII[][5] =
{
  {
    0x00, 0x00, 0x00, 0x00, 0x00    } // 20  
  ,{
    0x00, 0x00, 0x5f, 0x00, 0x00    } // 21 !
  ,{
    0x00, 0x07, 0x00, 0x07, 0x00    } // 22 "
  ,{
    0x14, 0x7f, 0x14, 0x7f, 0x14    } // 23 #
  ,{
    0x24, 0x2a, 0x7f, 0x2a, 0x12    } // 24 $
  ,{
    0x23, 0x13, 0x08, 0x64, 0x62    } // 25 %
  ,{
    0x36, 0x49, 0x55, 0x22, 0x50    } // 26 &
  ,{
    0x00, 0x05, 0x03, 0x00, 0x00    } // 27 '
  ,{
    0x00, 0x1c, 0x22, 0x41, 0x00    } // 28 (
  ,{
    0x00, 0x41, 0x22, 0x1c, 0x00    } // 29 )
  ,{
    0x14, 0x08, 0x3e, 0x08, 0x14    } // 2a *
  ,{
    0x08, 0x08, 0x3e, 0x08, 0x08    } // 2b +
  ,{
    0x00, 0x50, 0x30, 0x00, 0x00    } // 2c ,
  ,{
    0x08, 0x08, 0x08, 0x08, 0x08    } // 2d -
  ,{
    0x00, 0x60, 0x60, 0x00, 0x00    } // 2e .
  ,{
    0x20, 0x10, 0x08, 0x04, 0x02    } // 2f /
  ,{
    0x3e, 0x51, 0x49, 0x45, 0x3e    } // 30 0
  ,{
    0x00, 0x42, 0x7f, 0x40, 0x00    } // 31 1
  ,{
    0x42, 0x61, 0x51, 0x49, 0x46    } // 32 2
  ,{
    0x21, 0x41, 0x45, 0x4b, 0x31    } // 33 3
  ,{
    0x18, 0x14, 0x12, 0x7f, 0x10    } // 34 4
  ,{
    0x27, 0x45, 0x45, 0x45, 0x39    } // 35 5
  ,{
    0x3c, 0x4a, 0x49, 0x49, 0x30    } // 36 6
  ,{
    0x01, 0x71, 0x09, 0x05, 0x03    } // 37 7
  ,{
    0x36, 0x49, 0x49, 0x49, 0x36    } // 38 8
  ,{
    0x06, 0x49, 0x49, 0x29, 0x1e    } // 39 9
  ,{
    0x00, 0x36, 0x36, 0x00, 0x00    } // 3a :
  ,{
    0x00, 0x56, 0x36, 0x00, 0x00    } // 3b ;
  ,{
    0x08, 0x14, 0x22, 0x41, 0x00    } // 3c <
  ,{
    0x14, 0x14, 0x14, 0x14, 0x14    } // 3d =
  ,{
    0x00, 0x41, 0x22, 0x14, 0x08    } // 3e >
  ,{
    0x02, 0x01, 0x51, 0x09, 0x06    } // 3f ?
  ,{
    0x32, 0x49, 0x79, 0x41, 0x3e    } // 40 @
  ,{
    0x7e, 0x11, 0x11, 0x11, 0x7e    } // 41 A
  ,{
    0x7f, 0x49, 0x49, 0x49, 0x36    } // 42 B
  ,{
    0x3e, 0x41, 0x41, 0x41, 0x22    } // 43 C
  ,{
    0x7f, 0x41, 0x41, 0x22, 0x1c    } // 44 D
  ,{
    0x7f, 0x49, 0x49, 0x49, 0x41    } // 45 E
  ,{
    0x7f, 0x09, 0x09, 0x09, 0x01    } // 46 F
  ,{
    0x3e, 0x41, 0x49, 0x49, 0x7a    } // 47 G
  ,{
    0x7f, 0x08, 0x08, 0x08, 0x7f    } // 48 H
  ,{
    0x00, 0x41, 0x7f, 0x41, 0x00    } // 49 I
  ,{
    0x20, 0x40, 0x41, 0x3f, 0x01    } // 4a J
  ,{
    0x7f, 0x08, 0x14, 0x22, 0x41    } // 4b K
  ,{
    0x7f, 0x40, 0x40, 0x40, 0x40    } // 4c L
  ,{
    0x7f, 0x02, 0x0c, 0x02, 0x7f    } // 4d M
  ,{
    0x7f, 0x04, 0x08, 0x10, 0x7f    } // 4e N
  ,{
    0x3e, 0x41, 0x41, 0x41, 0x3e    } // 4f O
  ,{
    0x7f, 0x09, 0x09, 0x09, 0x06    } // 50 P
  ,{
    0x3e, 0x41, 0x51, 0x21, 0x5e    } // 51 Q
  ,{
    0x7f, 0x09, 0x19, 0x29, 0x46    } // 52 R
  ,{
    0x46, 0x49, 0x49, 0x49, 0x31    } // 53 S
  ,{
    0x01, 0x01, 0x7f, 0x01, 0x01    } // 54 T
  ,{
    0x3f, 0x40, 0x40, 0x40, 0x3f    } // 55 U
  ,{
    0x1f, 0x20, 0x40, 0x20, 0x1f    } // 56 V
  ,{
    0x3f, 0x40, 0x38, 0x40, 0x3f    } // 57 W
  ,{
    0x63, 0x14, 0x08, 0x14, 0x63    } // 58 X
  ,{
    0x07, 0x08, 0x70, 0x08, 0x07    } // 59 Y
  ,{
    0x61, 0x51, 0x49, 0x45, 0x43    } // 5a Z
  ,{
    0x00, 0x7f, 0x41, 0x41, 0x00    } // 5b [
  ,{
    0x02, 0x04, 0x08, 0x10, 0x20    } // 5c ¥
  ,{
    0x00, 0x41, 0x41, 0x7f, 0x00    } // 5d ]
  ,{
    0x04, 0x02, 0x01, 0x02, 0x04    } // 5e ^
  ,{
    0x40, 0x40, 0x40, 0x40, 0x40    } // 5f _
  ,{
    0x00, 0x01, 0x02, 0x04, 0x00    } // 60 `
  ,{
    0x20, 0x54, 0x54, 0x54, 0x78    } // 61 a
  ,{
    0x7f, 0x48, 0x44, 0x44, 0x38    } // 62 b
  ,{
    0x38, 0x44, 0x44, 0x44, 0x20    } // 63 c
  ,{
    0x38, 0x44, 0x44, 0x48, 0x7f    } // 64 d
  ,{
    0x38, 0x54, 0x54, 0x54, 0x18    } // 65 e
  ,{
    0x08, 0x7e, 0x09, 0x01, 0x02    } // 66 f
  ,{
    0x0c, 0x52, 0x52, 0x52, 0x3e    } // 67 g
  ,{
    0x7f, 0x08, 0x04, 0x04, 0x78    } // 68 h
  ,{
    0x00, 0x44, 0x7d, 0x40, 0x00    } // 69 i
  ,{
    0x20, 0x40, 0x44, 0x3d, 0x00    } // 6a j 
  ,{
    0x7f, 0x10, 0x28, 0x44, 0x00    } // 6b k
  ,{
    0x00, 0x41, 0x7f, 0x40, 0x00    } // 6c l
  ,{
    0x7c, 0x04, 0x18, 0x04, 0x78    } // 6d m
  ,{
    0x7c, 0x08, 0x04, 0x04, 0x78    } // 6e n
  ,{
    0x38, 0x44, 0x44, 0x44, 0x38    } // 6f o
  ,{
    0x7c, 0x14, 0x14, 0x14, 0x08    } // 70 p
  ,{
    0x08, 0x14, 0x14, 0x18, 0x7c    } // 71 q
  ,{
    0x7c, 0x08, 0x04, 0x04, 0x08    } // 72 r
  ,{
    0x48, 0x54, 0x54, 0x54, 0x20    } // 73 s
  ,{
    0x04, 0x3f, 0x44, 0x40, 0x20    } // 74 t
  ,{
    0x3c, 0x40, 0x40, 0x20, 0x7c    } // 75 u
  ,{
    0x1c, 0x20, 0x40, 0x20, 0x1c    } // 76 v
  ,{
    0x3c, 0x40, 0x30, 0x40, 0x3c    } // 77 w
  ,{
    0x44, 0x28, 0x10, 0x28, 0x44    } // 78 x
  ,{
    0x0c, 0x50, 0x50, 0x50, 0x3c    } // 79 y
  ,{
    0x44, 0x64, 0x54, 0x4c, 0x44    } // 7a z
  ,{
    0x00, 0x08, 0x36, 0x41, 0x00    } // 7b {
  ,{
    0x00, 0x00, 0x7f, 0x00, 0x00    } // 7c |
  ,{
    0x00, 0x41, 0x36, 0x08, 0x00    } // 7d }
  ,{
    0x10, 0x08, 0x08, 0x10, 0x08    } // 7e ←
  ,{
    0x78, 0x46, 0x41, 0x46, 0x78    } // 7f →
};

void LcdCharacter(char character)
{
  LcdWrite(LCD_D, 0x00);
  for (int index = 0; index < 5; index++)
  {
    LcdWrite(LCD_D, ASCII[character - 0x20][index]);
  }
  LcdWrite(LCD_D, 0x00);
}

void LcdClear(void)
{
  for (int index = 0; index < LCD_X * LCD_Y / 8; index++)
  {
    LcdWrite(LCD_D, 0x00);
  }
}

void LcdInitialise(void)
{
  pinMode(PIN_SCE,   OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_DC,    OUTPUT);
  pinMode(PIN_SDIN,  OUTPUT);
  pinMode(PIN_SCLK,  OUTPUT);

  digitalWrite(PIN_RESET, LOW);
 // delay(1);
  digitalWrite(PIN_RESET, HIGH);

  LcdWrite( LCD_CMD, 0x21 );  // LCD Extended Commands.
  LcdWrite( LCD_CMD, 0xBf );  // Set LCD Vop (Contrast). //B1
  LcdWrite( LCD_CMD, 0x04 );  // Set Temp coefficent. //0x04
  LcdWrite( LCD_CMD, 0x14 );  // LCD bias mode 1:48. //0x13
  LcdWrite( LCD_CMD, 0x0C );  // LCD in normal mode. 0x0d for inverse
  LcdWrite(LCD_C, 0x20);
  LcdWrite(LCD_C, 0x0C);
}

void LcdString(char *characters)
{
  while (*characters)
  {
    LcdCharacter(*characters++);
  }
}

void LcdWrite(byte dc, byte data)
{
  digitalWrite(PIN_DC, dc);
  digitalWrite(PIN_SCE, LOW);
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
  digitalWrite(PIN_SCE, HIGH);
}

// gotoXY routine to position cursor 
// x - range: 0 to 84
// y - range: 0 to 5

void gotoXY(int x, int y)
{
  LcdWrite( 0, 0x80 | x);  // Column.
  LcdWrite( 0, 0x40 | y);  // Row.  

}



void drawLine(void)
{
  unsigned char  j;  
   for(j=0; j<84; j++) // top
	{
          gotoXY (j,0);
	  LcdWrite (1,0x01);
  } 	
  for(j=0; j<84; j++) //Bottom
	{
          gotoXY (j,5);
	  LcdWrite (1,0x80);
  } 	

  for(j=0; j<6; j++) // Right
	{
          gotoXY (83,j);
	  LcdWrite (1,0xff);
  } 	
	for(j=0; j<6; j++) // Left
	{
          gotoXY (0,j);
	  LcdWrite (1,0xff);
  }

}

/***********************************
 * KEYPAD
 ***********************************/
#include <EEPROM.h>
#include <Keypad.h> //http://tronixstuff.wordpress.com/2011/11/12/tutorial-arduino-and-numeric-keypads-part-two/

//Password: http://arduino.cc/playground/Code/Password
//Keypad: http://arduino.cc/playground/Code/Keypad
//EEPROM: 
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] =
{
  { 
    '1','2','3','A'      }
  ,
  { 
    '4','5','6','B'      }
  ,
  { 
    '7','8','9','C'      }
  ,
  { 
    '*','0','#','D'      }
};
byte rowPins[ROWS] = {
  3, 2, 1, 0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
  4, 5, 6, 7}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char keypresses[4] = {
  0,0,0,0}; //store keypresses
int mode = 0; // 0 is normal, 1 is reprogram

void reprogram(char zone, int time) {
  LcdString("Zone is \n");
  // LcdString(zone);
  LcdString("New time is \n");
  //Convert ASCII to time
//  if (time < 64) {
//    LcdString(time - 48);
//  } 
//  else {
//    LcdString(time - 55);
//   }
}


/*******
GARDEN
*******/

const int valves[8] = { 5,6,7,8,9,10,11,12 }; //pins of valves on the Arduino

int times[8] = { 1,1,1,1,1,1,1,1 };  //stores times as minutes
//int valve;  //used for valve increment
int i;  //counter, used to increment from valve to valve

void water(int valve, int minutes) {
  Serial.print("Valve: \t");
  Serial.print(i+1); //the physical valve
  Serial.print("\tArduino Pin: \t");
  Serial.print(valve); //the valve pin
  Serial.print("\tTime in minutes: \t");
  Serial.println(minutes);  //time
  
  LcdClear();
  gotoXY(0,0);
  LcdString("Gardening: ");
  
  char msg[2];
  sprintf(msg, "%d", i+1);
  gotoXY(10,10);
  LcdString(msg);

  int remainSec;
  int remainMin;
  char timeremain[6];
  sprintf(timeremain, "%d:%d", remainMin, remainSec);
  gotoXY(20,20);
  LcdString(timeremain);
  
  unsigned long millitime = minutes * 6000;
  
  digitalWrite(valve, HIGH);
  delay(millitime);
  digitalWrite(valve, LOW);
  LcdClear();
  gotoXY(40,40);
  LcdString("Tank Refill");
  delay(6000); //wait a minute, this may need to be increased for real life
  
}

void sleepNow() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  ADCSRA &= ~(1 << ADEN);    // Disable ADC
  PRR = 0xFF;   // Power down functions
  sleep_mode();
}

void setup()
{
  LcdInitialise();
  LcdClear();
//  gotoXY(0,0);
//  LcdString("Initializing");
  
  Serial.begin(9600);
     
  for (i = 0; i < 8; i++) {
    pinMode(valves[i],OUTPUT);
  }
}

void loop()
{
  /*char key = keypad.getKey();

 if (key != NO_KEY) {
    
    //Enter Programming Mode 
    if ((key=='*') && (i==0)) {
      keypresses[i] = key;
      mode = 1;
      i++;
    }
    
    //Get values
    if ((mode == 1) && ((i>0) || (i<4)) && ((key != '*'))) {
      keypresses[i] = key;
      i++;
    }
    
    //Values completed
    if ((mode == 1) && (i==4)) {
 
      if ((keypresses[0] == '*') && (keypresses[3] == '#')) {
        reprogram(keypresses[1], keypresses[2]);
        LcdString("New Values Stored\n");
      }
      
      mode = 0;
      i = 0;
    }
  }*/
  
//  for (i = 0; i < 8; i++) {
//    water(valves[i], times[i]);
//   }
gotoXY(0,0);
if (i!=1) {
  gotoXY(0,0);
  LcdString("a");
  gotoXY(15,1);
  LcdString("b");
  gotoXY(30,2);
  LcdString("c");
  gotoXY(45,3);
  LcdString("d");
  gotoXY(60,4);
  LcdString("e");
  gotoXY(74,5);
  LcdString("f");

  i = 1;
}
//  sleepNow(); //powerdown
}



