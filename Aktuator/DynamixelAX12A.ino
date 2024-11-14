#include <DynamixelSerial3.h>

#define SERVO_ID  1
int Temperature,Voltage,Position; 

void setup(){
Dynamixel.begin(1000000,2);    // Inisialisasi servo di 1 Mbps dan Pin Control 2
Serial.begin(9600);           
}

void loop(){
  Temperature = Dynamixel.readTemperature(SERVO_ID);
  Voltage = Dynamixel.readVoltage(SERVO_ID);        
  Position = Dynamixel.readPosition(SERVO_ID);      

  Dynamixel.move(1,random(200,800));  // Menggerakan Servo Paramater ID, Degerees 
 
  Serial.print("Temperature: ");
  Serial.print(Temperature); Serial.print(" celcius, ");
  Serial.print("Voltage: ");
  Serial.print(float(Voltage)/10);Serial.print(" volts, ");
  Serial.print("Position: ");
  Serial.println(Position);
  delay(1000);
}