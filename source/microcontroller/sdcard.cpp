#include "sdcard.h"

bool SDCard::initCard() {
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("log:SD card failed or not present");
    validCard = false;
    return false;
  } else {
    Serial.println("log:SD card initialized.");
    validCard = true;
    return true;
  }
}

bool SDCard::readBool(const char *fileName, bool defaultValue) {
  if (!validCard) return defaultValue;
  
  File dataFile = SD.open(fileName, FILE_READ);

  if (dataFile) {
    String contents;
    
    while (dataFile.available()) {
      contents += (char)dataFile.read();
    }
    
    dataFile.close();

    return contents.toInt();
  }

  return defaultValue;
}

float SDCard::readFloat(const char *fileName, float defaultValue) {
  if (!validCard) return defaultValue;
  
  File dataFile = SD.open(fileName, FILE_READ);

  if (dataFile) {
    String contents;
    
    while (dataFile.available()) {
      contents += (char)dataFile.read();
    }
    
    dataFile.close();

    return contents.toFloat();
  }

  return defaultValue;
}

int SDCard::readInt(const char *fileName, int defaultValue) {
  if (!validCard) return defaultValue;
  
  File dataFile = SD.open(fileName, FILE_READ);

  if (dataFile) {
    String contents;
    
    while (dataFile.available()) {
      contents += (char)dataFile.read();
    }
    
    dataFile.close();

    return contents.toInt();
  }

  return defaultValue;
}
