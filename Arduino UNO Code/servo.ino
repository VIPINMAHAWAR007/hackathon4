#include <Servo.h> // including servo library.

Servo servo_1;
int t=999;
void setup (){
  Serial.begin(9600);
  servo_1.attach(3);
  servo_1.write (135);
}

void loop(){
  if(Serial.available()){
    t = Serial.read();
    Serial.println(t);
    if(t == 48){//Close-0
        servo_1.write (135);
    }
    else if(t == 49){//Open-1
        servo_1.write (45); // Servo will move to 45 degree angle.
    }
    delay(500);
  }
}
