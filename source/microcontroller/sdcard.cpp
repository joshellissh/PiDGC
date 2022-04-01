#include <SD.h>
#include "sdcard.h"

bool SDCard::initCard() {
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("LOG:SD card failed or not present");
    validCard = false;
    return false;
  } else {
    Serial.println("LOG:SD card initialized.");
    validCard = true;
    return true;
  }
}

float SDCard::readFloat(const char *fileName) {
  if (!validCard) return ERROR_FLOAT;
  
  File dataFile = SD.open(fileName, FILE_READ);

  if (dataFile) {
    String contents;
    
    while (dataFile.available()) {
      contents += (char)dataFile.read();
    }
    
    dataFile.close();

    return contents.toFloat();
  }

  return ERROR_FLOAT;
}

void SDCard::writeFloat(const char *fileName, float value) {
  File dataFile = SD.open(fileName, FILE_WRITE);

  if (dataFile) {
    dataFile.truncate();
    dataFile.print(value);
    dataFile.close();
  }
}
