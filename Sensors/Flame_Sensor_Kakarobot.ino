/*  
    Program ini mengambil referensi dari : https://github.com/kakarobot/Flame-Sensor-Kakarobot.git

    5v Sensor ke 5v Arduino  
    G Sensor ke Ground Arduino
    S2 Sensor ke pin 2 digital Arduino
    S1 Sensor ke pin 3 digital Arduino
    S0 Sensor ke pin 4 digital Arduino
    > Sensor ke pin A0 digital Arduino
    < Sensor ke pin A1 digital Arduino

*/

#include <FlameSensorKakarobot.h>

const int   S0 = 23, 
            S1 = 25, 
            S2 = 27, 
            adcDepan = A2, 
            adcBelakang = A1;

FlameSensorKakarobot flameSensor(S0, S1, S2, adcDepan, adcBelakang);

#define maxSensor 14

unsigned int dataSensor[maxSensor];
unsigned int refSensorDepan, refSensorBelakang;
unsigned int titikApi, nilaiTitikApi;

void setup() {
  Serial.begin(115200);  
}

void loop() {
  ReadSensor();
}

void bacaSensor(){
  for (int x = 0; x < maxSensor; x++) { 
    Serial.print(flameSensor.getValue(x));
    Serial.print(" ");
  }
  
  Serial.print("RefDep:");
  Serial.print(flameSensor.getRefF());
  Serial.print(" RefBel:");
  Serial.print(flameSensor.getRefB()); 
  Serial.print(" Posisi Api:");
  Serial.print(flameSensor.getPosition());
  Serial.print(" ADC Api:");
  Serial.println(flameSensor.getValuePosition());
  delay(100);
}

void ReadSensor(){
  char buf[30];
  
//  int pengkonversi = 16; //nilai pengkonversi 16 digunakan untuk mengubah nilai 4096 menjadi 255 (untuk modul Arduino adc 12bit misal esp32/esp8266)
  int pengkonversi = 4; //nilai pengkonversi 4 digunakan untuk mengubah nilai 1023 menjadi 255 (untuk modul Arduino adc 10bit misal arduino nano)
  for (int x = 0; x < maxSensor; x++) { 
    
    dataSensor[x] = flameSensor.getValue(x)/pengkonversi; 
  }
  refSensorDepan = flameSensor.getRefF()/pengkonversi;
  refSensorBelakang = flameSensor.getRefB()/pengkonversi;
  nilaiTitikApi = flameSensor.getValuePosition()/pengkonversi;
  titikApi = flameSensor.getPosition();
  
  Serial.println("Coba Sensor Api Kakarobot"); Serial.println(" ");
  
  sprintf(buf,"            %3d ",dataSensor[3]); Serial.println(buf);  
  sprintf(buf,"       %3d  %3d   %3d ",dataSensor[2],refSensorDepan, dataSensor[4]); Serial.println(buf); Serial.println(" ");
  sprintf(buf,"  %3d                 %3d ",dataSensor[1], dataSensor[5]); Serial.println(buf);  Serial.println(" ");
  sprintf(buf,"%3d         %3d         %3d ",dataSensor[0],titikApi,dataSensor[6]); Serial.println(buf); Serial.println(" ");
  sprintf(buf,"%3d         %3d         %3d ",dataSensor[13],nilaiTitikApi, dataSensor[7]); Serial.println(buf);Serial.println(" ");
  sprintf(buf,"  %3d                 %3d ",dataSensor[12], dataSensor[8]); Serial.println(buf); Serial.println(" ");
  sprintf(buf,"       %3d  %3d  %3d ",dataSensor[11],refSensorBelakang, dataSensor[9]); Serial.println(buf); 
  sprintf(buf,"            %3d ",dataSensor[10]); Serial.println(buf); Serial.println(" ");
  
  Serial.println(" "); 
  
  delay(100); 
}