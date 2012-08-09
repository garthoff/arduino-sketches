int i;
int randomnum;

void setup() {
  Serial.begin(9600);
  for (i=3; i<11; i++){
    pinMode(i, OUTPUT);
  }
  
  digitalWrite(3,1);
  digitalWrite(4,1);
  digitalWrite(5,0);
  digitalWrite(6,1);
  digitalWrite(7,1);
  digitalWrite(8,0);
  digitalWrite(9,1);
  digitalWrite(10,1);
}

void loop() {
//  for (i=2; i<10; i++) {
//    randomnum = random(0,2);
//    Serial.print(randomnum);
//    digitalWrite(i, randomnum);
//  }
//  Serial.println("");
//  Serial.println("--------------");
//  delay(5000);
}
