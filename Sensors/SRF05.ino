// Library URL : https://github.com/RobTillaart/SRF05.git

#include "SRF05.h"

const int pin_trigger = 10;
const int pin_echo    = 11;

SRF05 Sensor(trigger, echo);


void setup()
{
  Serial.begin(115200);
  Serial.println();  

  Sensor.setCorrectionFactor(1.035);
}


void loop()
{
  Serial.println(Sensor.getTime());             // uSec
  delay(100);
  Serial.println(Sensor.getMillimeter());       // mm
  delay(100);
  Serial.println(Sensor.getCentimeter(), 1);    // cm
  delay(100);
  Serial.println(Sensor.getMeter(), 2);         // m
  delay(100);
  Serial.println(Sensor.getInch(), 1);          // inch
  delay(100);
  Serial.println(Sensor.getFeet(), 2);          // feet
  delay(100);
  Serial.println(Sensor.getYard(), 2);          // yard
  delay(1000);
  Serial.println();
}
