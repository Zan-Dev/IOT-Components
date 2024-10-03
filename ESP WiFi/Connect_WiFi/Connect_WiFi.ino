#include <WiFi.h>

const char* ssid     = "SSID";  // SSID (Nama WiFi)
const char* password = "Password"; // Password WiFi

void setup(){
    Serial.begin(9600);
    Serial.print("Connecting to WiFi network: ");
    Serial.print(ssid);
    Serial.print(" ");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { 
        Serial.print(".");
        delay(500);    
    }

    Serial.println("");
    Serial.print("Berhasil Terkoneksi Ke - ");
    Serial.print(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop(){
    // Kode Kamu ...
}