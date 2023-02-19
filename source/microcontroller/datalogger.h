#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <USBHost_t36.h>
#include "Values.h"

extern USBHost usbHost;
extern USBDrive usbDrive;
extern USBFilesystem usbFS;

class DataLogger {
private:
  bool isMounted = false;
  bool hplEnabled = false;
  char fileFolder[64] = {0};
  char fileName[64] = {0};

  void initPrefix();
  void updateFilePointer(bool highPriorityLogging);
  void writeHeaders(const char *fileName);

public:
  void init();
  void writeValues(Values &values, bool highPriorityLogging);
};

#endif
