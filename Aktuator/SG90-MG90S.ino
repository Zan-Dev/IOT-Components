// Link URL Library : https://github.com/arduino-libraries/Servo.git
#include <Servo.h>

Servo myservo;
const int pin_servo = 10

void setup() {
  myservo.attach(pin_servo);
}

void loop() {
  for (int i = 0; i <= 180; i++) {
    Rotate_Servo(i, 15);
    if (i == 180)
    {
        for (int i = 180; i <= 0; i--)
        {
            Rotate_Servo(i, 15);
        }
        
    }
    
  }
}

void Rotate_Servo(degrees, speed){
    myservo.write(degrees);
    delay(speed);
}