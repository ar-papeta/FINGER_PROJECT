void FingerSensor_init(Adafruit_Fingerprint *f_Sensor) {
  // set the data rate for the sensor serial port
  f_Sensor->begin(115200);
  if (f_Sensor->verifyPassword()) 
  {
    Serial.println("Found fingerprint sensor!");
  } 
  else 
  {
    Serial.println("Did not find fingerprint sensor :(");
  } 
}




int checkFinger(Adafruit_Fingerprint *f_Sensor) {
  uint8_t f_status = f_Sensor->getImage();
  if (f_status != FINGERPRINT_OK)  return -1;

  f_status = f_Sensor->image2Tz();
  if (f_status != FINGERPRINT_OK)  return -1;

  f_status = f_Sensor->fingerFastSearch();
  if (f_status != FINGERPRINT_OK)  return -1;

  // found a match!
  //Serial.print("Found ID #"); Serial.println(f_Sensor->fingerID);
  if (f_Sensor->fingerID != 1) {
    //openDoor(1);
    //HTTPS_GET_Request(SERVER, "ESP/", "FingerEnter/",(int)finger_IN.fingerID);
  }
  return f_Sensor->fingerID;
}



bool addFinger(Adafruit_Fingerprint *f_Sensor, uint8_t id) {
  /*TO DO add log info transmit to server !!!and timout for while operations!!!*/
  int f_status = -1;
  while (f_status != FINGERPRINT_OK) {
    f_status = f_Sensor->getImage();
    switch (f_status) {
      case FINGERPRINT_OK:
        break;
      case FINGERPRINT_NOFINGER:
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        break;
      case FINGERPRINT_IMAGEFAIL:
        break;
      default:
        break;
    }
  }

  f_status = f_Sensor->image2Tz(1);
  /*TO DO add log info transmit to server*/
  switch (f_status) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return false;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return false;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return false;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return false;
    default:
      //Serial.println("Unknown error");
      return false;
  }
  //TO DO add service info
  //Уберите палец с дачтика
  //Wait and go
  //Serial.println("Wait 2 seconds");

  delay(2000); //Ждем 2 сек, чтоб пользователь убрал палец

  f_status = 0;
  while (f_status != FINGERPRINT_NOFINGER) {
    f_status = f_Sensor->getImage();
  }
  f_status = -1;
  //Serial.println("Place same finger again");  //Повторно приложите палец
  while (f_status != FINGERPRINT_OK) {
    f_status = f_Sensor->getImage();
    switch (f_status) {
      case FINGERPRINT_OK:
        //Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        //Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        //Serial.println("Imaging error");
        break;
      default:
        //Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  f_status = f_Sensor->image2Tz(2);
  switch (f_status) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return false;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return false;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return false;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return false;
    default:
      //Serial.println("Unknown error");
      return false;
  }

  // OK converted!
  //Serial.print("Creating model for #");  Serial.println(id);
  f_status = f_Sensor->createModel();
  if (f_status == FINGERPRINT_OK) {
    //Serial.println("Personal print added!");
  } else if (f_status == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return false;
  } else if (f_status == FINGERPRINT_ENROLLMISMATCH) {
    //Serial.println("Fingerprints did not match");
    return false;
  } else {
    //Serial.println("Unknown error");
    return false;
  }

  f_status = f_Sensor->storeModel(id);
  if (f_status == FINGERPRINT_OK) {
    Serial.println("Personal finger stored!");
    return true;
  } else if (f_status == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return false;
  } else if (f_status == FINGERPRINT_BADLOCATION) {
    //Serial.println("Could not store in that location");
    return false;
  } else if (f_status == FINGERPRINT_FLASHERR) {
    //Serial.println("Error writing to flash");
    return false;
  } else {
    //Serial.println("Unknown error");
    return false;
  }
}

bool deleteFinger(Adafruit_Fingerprint *f_Sensor, uint8_t id) {
  uint8_t f_status = -1;
  f_status = f_Sensor->deleteModel(id);

  if (f_status == FINGERPRINT_OK) {
    Serial.print("FINGERPRINT SENSOR: User with ID "); Serial.print(id); Serial.println(" removed");
    return true;
  } else if (f_status == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return false;
  } else if (f_status == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    return false;
  } else if (f_status == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return false;;
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(f_status, HEX);
    return false;
  }
}
