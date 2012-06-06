#include <EEPROM.h>

char maker[20] = "Bill Seremetis";
char email[20] = "bill@seremetis.net";
char ver[10] = "prototype";
char snumber[10] = "000000001";

//char productData[61] = maker + email + ver + snumber ;



void setup(){ 
  Serial.begin(9600);
}

int address = 0;
byte value;

void loop() {
  // read a byte from the current address of the EEPROM
  value = EEPROM.read(address);
  
  Serial.print(address);
  Serial.print("\t");
  Serial.print(value, DEC);
  Serial.println();
  
  // advance to the next address of the EEPROM
  address = address + 1;
  
  if (address == 61)
    address = 0;
    
  delay(500);
}
