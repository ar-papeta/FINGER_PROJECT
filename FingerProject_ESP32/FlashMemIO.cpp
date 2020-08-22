#include "EEPROM.h"
#include "FlashMemIO.h"

/*
                  USED MEMORY MAP

0...100       - Sevise cfg bytes
      0       - SELF_CONN_FLAG without external WiFi AP mode
      1       - EX_WIFI_CONNECTED use external WiFi STA status flag
      2...100 - reserved

101...101+EEPROM_SIZE_SSID  - SSID wifi name bytes

101+EEPROM_SIZE_SSID...101+EEPROM_SIZE_SSID+EEPROM_SIZE_PASSWORD -Password wifi bytes

*/
int CFG_START_AREA_ADDR = 0;
int SSID_START_ADDR = CFG_START_AREA_ADDR + EEPROM_SIZE_CFG_AREA;
int PASSWORD_START_ADDR = SSID_START_ADDR + EEPROM_SIZE_SSID;

bool EEPROM_init(){
  return EEPROM.begin(EEPROM_SIZE_SSID + EEPROM_SIZE_PASSWORD + EEPROM_SIZE_CFG_AREA) ? true : false;
}


// the sample text which we are storing in EEPROM
void EEPROM_Write(char *dataToWrite, uint16_t dataSize, uint16_t dataStartAddr){
  uint16_t addr = dataStartAddr;
  Serial.print("\n EEPROM data write:");
  for (int i = 0; i < dataSize; i++) {
      EEPROM.write(addr, dataToWrite[i]);
      addr++;
  }
  EEPROM.commit();
}


void EEPROM_Read(char dataBuff[], uint16_t dataSize, uint16_t dataStartAddr){
  uint16_t addr = dataStartAddr;
  Serial.print("\n EEPROM data read:");
  for (int i = 0; i < dataSize; i++) {
        dataBuff[i]= EEPROM.read(addr);
        addr++;
        Serial.print(dataBuff[i]);
        if(dataBuff[i] == '\0')
        return;
  }
}
