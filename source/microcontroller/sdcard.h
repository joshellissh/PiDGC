#ifndef SDCARD_H
#define SDCARD_H

#include <SD.h>

#define TRIP_FILE     "trip.txt"
#define ODOMETER_FILE "odometer.txt"
#define PPM_FILE      "ppm.txt"
#define BLINKER_FILE  "blinkerSound.txt"
#define DIMMING_FILE  "screenDimming.txt"

class SDCard {
private:
  bool validCard = false;

public:
  bool initCard();
  
  bool readBool(const char *fileName, bool defaultValue);
  float readFloat(const char *fileName, float defaultValue);
  int readInt(const char *fileName, int defaultValue);

  template <typename T> void writeValue(const char *fileName, T value) {
    File dataFile = SD.open(fileName, FILE_WRITE);
  
    if (dataFile) {
      dataFile.truncate();
      dataFile.print(value);
      dataFile.close();
    }
  }
};

#endif
