#include "HTMLPages.h"

const char* host = "finger";
const char* self_ssid = "Finger config";
const char* self_password = NULL;

char wifi_password[EEPROM_SIZE_PASSWORD];
char wifi_SSID[EEPROM_SIZE_SSID];

char isSelfConnect;
char isWiFiSTAConnect;

/*
 * setup function
 */
void ESP_WebServer_setup(void) {
  
  EEPROM_Read(&isSelfConnect, 1, EEPROM_SELF_CONN_FLAG_ADDR);
  EEPROM_Read(&isWiFiSTAConnect, 1, EEPROM_EX_WIFI_FLAG_ADDR);
  EEPROM_Read(wifi_SSID, EEPROM_SIZE_SSID, SSID_START_ADDR);
  EEPROM_Read(wifi_password, EEPROM_SIZE_PASSWORD, PASSWORD_START_ADDR);
  
  // Connect to WiFi network
  if (isSelfConnect == '1') {
    //WIFI_AP_STA
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(self_ssid, self_password);
    Serial.println("");
    Serial.print("Connected to self SSID: ");
    Serial.println(self_ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
  } else if (isWiFiSTAConnect == '1') {
    int tryCount = 0;
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_SSID, wifi_password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      if(tryCount == 15){
          Serial.println("Something wrong with WiFi. 15 connection attempts faild");
          break;
      }
      else{
        Serial.println("IP address: ");  //  "IP-адрес: "
        Serial.println(WiFi.localIP());
      }
    }
  }

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) {  // http://finger.local
    Serial.println("Error setting up MDNS responder! restarting...");
    ESP.restart();
  } else {
    Serial.print("mDNS responder started at: ");
    Serial.print("http://");
    Serial.print(host);
    Serial.println(".local");
    MDNS.addService("http", "tcp", 80);
  }

  /*return index page which is stored in serverIndex */

  server.on("/", HTTP_POST, []() {
    if (server.hasArg("psw") && server.hasArg("ssid")) {
      strcpy(wifi_SSID, server.arg("ssid").c_str());
      strcpy(wifi_password, server.arg("psw").c_str());
      int tryCount = 0;
      WiFi.begin(wifi_SSID, wifi_password);
      while(WiFi.status() != WL_CONNECTED){
        tryCount++;
        delay(500);
        Serial.print(".");
        if(tryCount == 15){
          Serial.println("Something wrong with WiFi. 15 connection attempts faild");
          break;
        }
        
      }
      if(tryCount < 15){
        isSelfConnect = '0';
        isWiFiSTAConnect = '1';
        EEPROM_Write(&isSelfConnect, 1, EEPROM_SELF_CONN_FLAG_ADDR);
        EEPROM_Write(&isWiFiSTAConnect, 1, EEPROM_EX_WIFI_FLAG_ADDR);
        
        EEPROM_Write(wifi_SSID, EEPROM_SIZE_SSID, SSID_START_ADDR);
        EEPROM_Write(wifi_password, EEPROM_SIZE_PASSWORD, PASSWORD_START_ADDR);
        server.sendHeader("Connection", "close");  
        server.send(200, "text/html", HomePage);
        ESP.restart();
      }
      else {
        WiFi.mode(WIFI_AP);
        WiFi.softAP(self_ssid, self_password);
        server.sendHeader("Connection", "close");  
        server.send(200, "text/html", RegPage);
      }
      Serial.println(server.arg("psw"));
      Serial.println(server.arg("ssid"));
    }
    
  });

  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");  
    
    if(isWiFiSTAConnect == '1'){
      Serial.println("HomePage");
      server.send(200, "text/html", HomePage);
    } 
    else if(isSelfConnect == '1'){
      Serial.println("RegPage");
      server.send(200, "text/html", RegPage);
    }
    else {
      Serial.println("Wrong Home page flags !!!");
    }   
  });

  
                    /*REG PAGE BLOCK*/
  server.on("/registration", HTTP_GET, []() {
    server.sendHeader("Connection", "close");  
    server.send(200, "text/html", RegPage); 
  });

  server.on("/registration", HTTP_POST, []() {
    if (server.hasArg("noreg")){
      WiFi.mode(WIFI_AP);
      WiFi.softAP(self_ssid, self_password);
      isSelfConnect = '0';
      isWiFiSTAConnect = '1';
      EEPROM_Write(&isSelfConnect, 1, EEPROM_SELF_CONN_FLAG_ADDR);
      EEPROM_Write(&isWiFiSTAConnect, 1, EEPROM_EX_WIFI_FLAG_ADDR);
      Serial.println("Offline mode activated");
      Serial.print("IP address: ");   
      Serial.println(WiFi.softAPIP());
      server.sendHeader("Connection", "close");  
      server.send(200, "text/html", HomePage);
    }

    if (server.hasArg("psw") && server.hasArg("ssid")) {
      strcpy(wifi_SSID, server.arg("ssid").c_str());
      strcpy(wifi_password, server.arg("psw").c_str());
      int tryCount = 0;
      WiFi.begin(wifi_SSID, wifi_password);
      while(WiFi.status() != WL_CONNECTED){
        tryCount++;
        delay(500);
        Serial.print(".");
        if(tryCount == 15){
          Serial.println("Something wrong with WiFi. 15 connection attempts faild");
          break;
        }
        
      }
      if(tryCount < 15){
        isSelfConnect = '0';
        isWiFiSTAConnect = '1';
        EEPROM_Write(&isSelfConnect, 1, EEPROM_SELF_CONN_FLAG_ADDR);
        EEPROM_Write(&isWiFiSTAConnect, 1, EEPROM_EX_WIFI_FLAG_ADDR);
        
        EEPROM_Write(wifi_SSID, EEPROM_SIZE_SSID, SSID_START_ADDR);
        EEPROM_Write(wifi_password, EEPROM_SIZE_PASSWORD, PASSWORD_START_ADDR);
        server.sendHeader("Connection", "close");  
        server.send(200, "text/html", HomePage);
        ESP.restart();
      }
      else {
        WiFi.mode(WIFI_AP);
        WiFi.softAP(self_ssid, self_password);
        server.sendHeader("Connection", "close");  
        server.send(200, "text/html", RegPage);
      }
      Serial.println(server.arg("psw"));
      Serial.println(server.arg("ssid"));
    }
  });





                    /*ADD USER BLOCK*/
  server.on("/addUser", HTTP_GET, []() {
    server.sendHeader("Connection", "close");  
    server.send(200, "text/html", AddUserPage);
  });
  server.on("/addUser", HTTP_POST, []() {
    server.sendHeader("Connection", "close");  
    server.send(200, "text/html", "<h1>Page with instruction for adding!!!<\h1>");
    if (server.hasArg("usrId") && server.hasArg("usrName") && server.hasArg("usrStatus") && server.hasArg("usrKey")){
      int userId = -1;
      sscanf(server.arg("usrId").c_str(), "%d", &userId); 
      addFinger(&finger_OUT, (uint8_t)userId);
    }
    else{
      Serial.println("Something wrong with reques for add finger");
      }
  });



                                      /*DELETE USER BLOCK*/
  server.on("/deleteUser", HTTP_GET, []() {
    server.sendHeader("Connection", "close");  
    server.send(200, "text/html", DeleteUserPage);
  });
  server.on("/deleteUser", HTTP_POST, []() {
    server.sendHeader("Connection", "close");  
    server.send(200, "text/html", "<h1>Page with instruction for delete!!!<\h1>");
    Serial.println(server.arg("delUsrId"));
    if (server.hasArg("delUsrId")){
      int userId = -1;
      sscanf(server.arg("delUsrId").c_str(), "%d", &userId);
      deleteFinger(&finger_OUT, (uint8_t)userId);
    }
    else{
      Serial.println("Something wrong with reques for delete finger");
      }
  });


                                    /*OTA UPDATE USER BLOCK*/
  server.on("/OTAUpdate", HTTP_GET, []() {
    server.sendHeader("Connection", "close");  
    server.send(200, "text/html", OTAUpdatePage);
  });
  /*handling uploading firmware file */
  server.on(
      "/update", HTTP_POST,
      []() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
        ESP.restart();
      },
      []() {
        HTTPUpload& upload = server.upload();
        if (upload.status == UPLOAD_FILE_START) {
          Serial.printf("Update: %s\n", upload.filename.c_str());
          if (!Update.begin(
                  UPDATE_SIZE_UNKNOWN)) {  // start with max available size
            Update.printError(Serial);
          }
        } else if (upload.status == UPLOAD_FILE_WRITE) {
          /* flashing firmware to ESP*/
          if (Update.write(upload.buf, upload.currentSize) !=
              upload.currentSize) {
            Update.printError(Serial);
          }
        } else if (upload.status == UPLOAD_FILE_END) {
          if (Update.end(
                  true)) {  // true to set the size to the current progress
            Serial.printf("Update Success: %u\nRebooting...\n",
                          upload.totalSize);
          } else {
            Update.printError(Serial);
          }
        }
      });
  server.begin();
}
