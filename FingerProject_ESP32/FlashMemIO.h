
#ifndef FLASH_MEM_IO
#define FLASH_MEM_IO

#define EEPROM_SIZE_SSID     128
#define EEPROM_SIZE_PASSWORD 128
#define EEPROM_SIZE_CFG_AREA 100

#define EEPROM_SELF_CONN_FLAG_ADDR    0
#define EEPROM_EX_WIFI_FLAG_ADDR      1

extern int CFG_START_AREA_ADDR;
extern int SSID_START_ADDR;
extern int PASSWORD_START_ADDR;

bool EEPROM_init();
void EEPROM_Write(char *dataToWrite, uint16_t dataSize, uint16_t dataStartAddr);
void EEPROM_Read(char dataBuff[], uint16_t dataSize, uint16_t dataStartAddr);

#endif
