#ifndef SDCARD_H
#define SDCARD_H

#define ERROR_FLOAT -9999.0f

class SDCard {
private:
  bool validCard = false;

public:
  bool initCard();
  float readFloat(const char *fileName);
  void writeFloat(const char *fileName, float value);
};

#endif
