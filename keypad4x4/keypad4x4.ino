#include <EEPROM.h>
#include <Keypad.h> //http://tronixstuff.wordpress.com/2011/11/12/tutorial-arduino-and-numeric-keypads-part-two/

//Password: http://arduino.cc/playground/Code/Password
//Keypad: http://arduino.cc/playground/Code/Keypad
//EEPROM: 
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] =
{
  { '1','2','3','A'  },
  { '4','5','6','B'  },
  { '7','8','9','C'  },
  { '*','0','#','D'  }
};
byte rowPins[ROWS] = {
  5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
  6, 7, 8, 9}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int valves[8] = { 0,1,2,3,4,5,6,7 }; //pins of valves on the Arduino
int times[8];  //stores times as minutes
int valve;  //used for valve increment

char keypresses[4] = {0,0,0,0}; //store keypresses
int mode = 0; // 0 is normal, 1 is reprogram
int i = 0; //just a counter

void reprogram(char zone, int time) {
  Serial.print("Zone is ");
  Serial.println(zone);
  Serial.print("New time is ");
  //Convert ASCII to time
  if (time < 64) {
    Serial.println(time - 48);
  } else {
    Serial.println(time - 55);
  }
}

void water(int valve, int minutes) {
  unsigned long millitime = minutes * 60000;
  digitalWrite(valve, HIGH);
  delay(millitime);
  digitalWrite(valve, LOW);
  delay(60000); //wait a minute, this may need to be increased for real life
}

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  char key = keypad.getKey();

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
      Serial.println("In 3rd IF");
      Serial.print(keypresses[0]);
      Serial.print(keypresses[1]);
      Serial.print(keypresses[2]);
      Serial.println(keypresses[3]);
      
      if ((keypresses[0] == '*') && (keypresses[3] == '#')) {
        reprogram(keypresses[1], keypresses[2]);
        Serial.println("New Values Stored");
      }
      
      mode = 0;
      i = 0;
    }
  }

}
