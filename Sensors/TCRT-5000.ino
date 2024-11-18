/*  
    VCC Sensor ke 5v Arduino  
    GND Sensor ke Ground Arduino
    D0 Sensor ke pin 7 digital Arduino
    A0 Sensor ke pin A0 digital Arduino    

*/

conts int   pinIRD = 7;
            pinIRA = A0;

void setup(){
    serial.begin(9600);
    pinMode(pinIRD, INPUT);
    pinMode(pinIRA, INPUT);
}

void loop(){
    IRA = analogRead(pinIRA);
    IRD = digitalRead(pinIRD);

    Serial.print("Nilai Pembacaan Analog");
    Serial.println(IRA);
    Serial.print("Nilai Pembacaan Digital");
    Serial.println(IRD);
}