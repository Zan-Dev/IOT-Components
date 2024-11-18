/*

*/

#include "DFRobot_PH.h"
#include <EEPROM.h>

const int PHPin = A1
float voltage, phValue, temperature;

DFRobot_PH ph;

void setup(){
    Serial.begin(115200);
    ph.begin();
}

void loop(){
    ReadPH();
}

void ReadPH(){
    static unsigned long timepoint = millis();
    if(millis() - timepoint > 1000U){
        timepoint = millis();

        temperature = ReadTemperature();
        voltage = analogread(PHPin)/1024.0*5000;
        phValue = ph.readPH(voltage, temperature);
        Serial.print("PH : ");
        Serial.println(phValue);
    }
}

void ReadTemperature(){
    // Program sensor suhu
}