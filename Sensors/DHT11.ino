/*
    Link Library : https://github.com/markruys/arduino-DHT.git

    VCC sensor ke 5V Microcontroller
    GND sensor ke Gnd
    Data sensor ke Pin 2

*/

#include "DHT.h"

DHT dht;
const int dataPin = 2;
float humidity, temperature;

void setup(){
    Serial.begin(9600);
    dht.setup(dataPin);
}

void loop(){
    ReadDHT();
}

void ReadDHT(){
    humidity = dht.getHumidity();
    temperature = dht.getTemperature();

    Serial.println(dht.getStatusString());
    Serial.print("Humidity : "); Serial.println(humidity, 1);
    Serial.print("Temperature : "); Serial.println(temperature, 1);
    delay(500);
}