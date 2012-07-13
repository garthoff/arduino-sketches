/*
Valve, EEPROM Address
 A, 101
 */


#include <EEPROM.h>

boolean programming_mode = 0;

void write_info_to_device() {
  String maker = "Bill Seremetis";
  String email = "bill@seremetis.net";
  String ver = "prototype";
  String serial_number = "P0001";

  for (int address = 0; address < 20; address++) {
    EEPROM.write(address, maker[address]);
  }

  for (int address = 20; address < 50; address++) {
    EEPROM.write(address, email[address-20]);
  }

  for (int address = 50; address < 60; address++) {
    EEPROM.write(address, ver[address-50]);
  }

  for (int address = 60; address < 65; address++) {
    EEPROM.write(address, serial_number[address-60]);
  }
}

void reprogram_garden() {
  programming_mode = 1;
  int valve_to_reprogram;
  int new_time;
  Serial.println("Choose valve");
  if (Serial.available()) {
    valve_to_reprogram = Serial.read();
  }
  Serial.println("Enter new time in seconds");
  if (Serial.available()) {
    new_time = Serial.read();
  }
  Serial.print("Valve ");
  Serial.print(valve_to_reprogram);
  Serial.print(" will be set to ");
  Serial.print(new_time);
  Serial.println(" seconds.");
  programming_mode = 0;
}


void menu_print() {
  Serial.println("Please make a choice:");
  Serial.println("1. Show Garden Program");
  Serial.println("2. Reprogram Garden");
  Serial.println("i. Display Device Information");
  Serial.println("f. Flash Device Information");
  Serial.println("Type h or ? to reprint the menu.");
}

//void clear_terminal_screen() {
// Serial.write(27); //Print "esc"
// Serial.write("[2J");
//}

void display_device_info() {
  byte value;

  Serial.println("Device Information");
  Serial.println("------------------");
  for (int address = 0; address < 65; address++) {
    if (address == 0) {
      Serial.print("maker: ");
    }
    if (address == 20) {
      Serial.println();
      Serial.print("email: ");
    }
    if (address == 50) {
      Serial.println();
      Serial.print("version: ");
    }
    if (address == 60) {
      Serial.println();
      Serial.print("SN: ");
    }
    value = EEPROM.read(address);
    Serial.write(value);
  }
}

void menu_choice(char choice) {
  switch(choice) {
  case '2':
    reprogram_garden();
    break;
  case 'i':
    display_device_info();
    break;
  case 'f':
    write_info_to_device();
    break;
  case 'h':
    menu_print();
    break;
  case '?':
    menu_print();
    break;
  }
}

void setup() {
  Serial.begin(9600);
  menu_print();
}

void loop() {
  if (Serial.available()) {
    menu_choice(Serial.read());
  }
}

