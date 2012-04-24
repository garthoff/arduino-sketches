const int LED = 9; //led pin
const int BUTTON = 7; //button pin
int val = 0; //store BUTTON state
int old_val = 0;
int state = 0;
int bright = 128;
unsigned long startTime = 0;


void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
  Serial.begin(9600);
}

void loop() {
  val = digitalRead(BUTTON);
  Serial.println(val);
  
  if (val == HIGH && old_val == LOW) {
    state = 1 - state;
    startTime = millis();
    delay(10);
  }
  
  if (val == HIGH && old_val == HIGH) {
    if (state == 1 && (millis() - startTime) > 500) {
      bright++;
      delay(10);
      if (bright > 255) { bright = 0; }
    }
  }
    
  old_val = val;
  
  if (state == 1) {
    analogWrite(LED,bright);
  } else {
    analogWrite(LED, LOW);
  }
}
