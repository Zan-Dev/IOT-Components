#include <WiFiManager.h>

WiFiManager wm;

void setup() {
    WiFi.mode(WIFI_STA);
    Serial.begin(9600);                

    bool res;    
    res = wm.autoConnect("AutoConnectAP","password");

    if(!res){
        Serial.println("Gagal Terhubung");
    } else {
        Serial.println("Terhubung : ");    
    }  

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

}

void loop() {
    // Kode Kamu
}