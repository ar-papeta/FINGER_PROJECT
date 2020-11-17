#include <ArduinoJson.h>
#include <FPM.h>
/* Read, delete and move a fingerprint template within the database */

#define FINGER_HUART Serial2

FPM finger(&FINGER_HUART);
FPM_System_Params params;

/* this should be equal to the template size for your sensor but
 * some sensors have 512-byte templates while others have template sizes as
 * high as 1024 bytes. So check the printed result of read_template()
 * to determine the case for your module and adjust the needed buffer
 * size below accordingly. If it doesn't work at first, try increasing
 * this value to 1024
 */
#define BUFF_SZ     512
StaticJsonDocument<1000> jsonDocument;
uint8_t template_buffer[BUFF_SZ];

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
