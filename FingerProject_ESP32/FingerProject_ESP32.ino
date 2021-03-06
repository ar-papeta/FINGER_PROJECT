#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <Adafruit_Fingerprint.h>
#include "FlashMemIO.h"

WebServer server(80); /* Server for OTA and finger config*/

Adafruit_Fingerprint finger_IN = Adafruit_Fingerprint(&Serial);
Adafruit_Fingerprint finger_OUT = Adafruit_Fingerprint(&Serial2);

//char* ssid = "Young poets";
//char* password = "524room524";
//char* ssid = "Tytul";
//char* password = "22559988";
char *initValue_SELF = "1";
char *initValue_WIFI = "0";

void setup() {
  
  Serial.begin(57600);
  //Serial1.begin(57600,SERIAL_8N1, 2, 4); //Baud rate, parity mode, RX, TX
  Serial2.begin(115200);
  
  //FingerSensor_init(&finger_IN);
  FingerSensor_init(&finger_OUT);
  EEPROM_init();
  delay(100);
  //EEPROM_Write(initValue_SELF, 1, EEPROM_SELF_CONN_FLAG_ADDR);
  //EEPROM_Write(initValue_WIFI, 1, EEPROM_EX_WIFI_FLAG_ADDR);
  ESP_WebServer_setup();

}

void loop() {
  
  server.handleClient();
  //checkFinger(&finger_IN);
  int a = checkFinger(&finger_OUT);
  //if(a > 0)
    Serial.println(a);
  
}
