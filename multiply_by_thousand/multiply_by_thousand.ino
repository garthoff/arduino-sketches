int i = 0;
const int valves[7] = { 
  5,6,7,8,9,10,11 }; //pins of valves on the Arduino

int human_times[7] = { 
  60,90,150,120,120,120,60 };  //stores times as seconds

unsigned long water_times[7];

void setup()
{
  Serial.begin(9600);

  for (i = 0; i < 7; i++) {
water_times[i] = human_times[i] * 1000;
    Serial.print("Valve: ");
    Serial.print(i+1); //the physical valve
    Serial.print(" human time: ");
    Serial.println(human_times[i]);  //time    
    Serial.print(" Time in milliseconds: ");
    Serial.println(water_times[i]);  //time
  }
}

void loop(){
}


