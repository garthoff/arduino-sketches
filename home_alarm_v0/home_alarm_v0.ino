/*
  This might become a home alarm in the near future.
  At the moment it is just work in progress.
  
  Reads a digital input on pin 2, prints the result to the serial monitor 

*/

int LED = 13;
int flag = 0;

void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  int sensorValue = digitalRead(2);
  if(sensorValue == LOW) {
    if(flag != 0){
      flag = 0;
      Serial.println("Intruder Alert on zone X ");
      Serial.println(millis()/1000);
      digitalWrite(LED, HIGH);
    }
  } else {
    digitalWrite(LED, LOW);
    if(flag != 1){
      flag = 1;
      Serial.println("All zones Ok!");
    }
  }
}



