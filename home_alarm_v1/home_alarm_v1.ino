/*
  This might become a home alarm in the near future.
  At the moment it is just work in progress.
  
  Reads a digital input on pin 2, prints the result to the serial monitor 

*/

int LED = 11;
int BUZZ = 10;
int BUTTON = 2;
int pwm_value = 0;
int pwm_stepper = 1;
int manual_stop_new = 0;
int manual_stop_old = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(BUZZ, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop() {
  int zone0 = analogRead(A0);
  

  
  if(zone0 < 1000) {
    if (digitalRead(BUTTON) != 0) {
      manual_stop_old = 1;
    }
    if (manual_stop_old == 0) {
      analogWrite(BUZZ, pwm_value);
      analogWrite(LED, pwm_value);
    } else {
      analogWrite(BUZZ, 0);
      analogWrite(LED, 0);
    }    
    pwm_value = pwm_value + pwm_stepper;
    Serial.println(pwm_value);
    
    if (pwm_value == 0 || pwm_value == 255) {
      pwm_stepper = -pwm_stepper;
    }
  } else {
    analogWrite(BUZZ, 0);
    analogWrite(LED, 0);
    manual_stop_old = 0;
  }
}



