#include <Entropy.h>
#include "datalogger.h"

void DataLogger::initPrefix() {
  Entropy.Initialize();

  fileFolder[0] = Entropy.random(97, 123);
  fileFolder[1] = Entropy.random(97, 123);
  fileFolder[2] = Entropy.random(97, 123);
}

void DataLogger::init() {
  initPrefix();
  usbHost.begin();
}

void DataLogger::writeHeaders(const char *fileName) {
  File dataFile = usbFS.open(fileName, FILE_WRITE);
  
  if (dataFile) {
    dataFile.truncate();
    dataFile.println("time,rpm,mph,map,boost,ect,iat,timing_advance,tps,fpp,os1cafer,stftb1,ltftb1,tip,oil_pressure");
    dataFile.close();
  }
}

void DataLogger::writeValues(Values &values, bool highPriorityLogging) {
  if (!usbFS) {
    usbHost.Task();
    isMounted = false;
    return;
  }

  if (!isMounted) {
    isMounted = true;
    memset(fileName, '\0', 64);
    Serial.println("log:USB connected");
  }

  // Update log file name if needed
  updateFilePointer(highPriorityLogging);

  // Calculate timestamp
  unsigned long currentMillis = millis();
  unsigned long seconds = currentMillis / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  currentMillis %= 1000;
  seconds %= 60;
  minutes %= 60;
  hours %= 24;

  File dataFile = usbFS.open(fileName, FILE_WRITE);
  char output[256] = {0};
  sprintf(output, 
          "%lu:%lu:%lu:%lu,%d,%d,"  // time (ulong), rpm (int), mph (int)
          "%f,%f,%d,"   // manifold absolute pressure (float), boost pressure (float), coolant temp (int)
          "%d,%f,%f,"   // intake air temp (int), timing advance (float), throttle position (float)
          "%f,%f,%f,"   // foot pedal position (float), oxy sensor 1 commanded air-fuel equivalence ratio (float), short term fuel trim bank 1 (float)
          "%f,%f,%f",   // long term fuel trim bank 1 (float), throttle inlet pressure (float), oil pressure (float)
          hours, minutes, seconds, currentMillis, values.rpm, values.mph.get(), 
          values.manifoldAbsolutePressure, values.boostPressure, values.coolantTemp, 
          values.intakeAirTemp, values.timingAdvance, values.throttlePosition, 
          values.footPedalPosition, values.oxySensor1AFER, values.shortTermFuelTrimB1, 
          values.longTermFuelTrimB1, values.throttleInletPressure, values.oilPressure);
          
  dataFile.println(output);
  dataFile.close();
}

void DataLogger::updateFilePointer(bool highPriorityLogging) {
  unsigned long currentMillis = millis();

  if (currentMillis % 120000 == 0 || strlen(fileName) == 0 || highPriorityLogging != hplEnabled) {
    if (highPriorityLogging) {
      hplEnabled = true;
      char oldFolder[64] = {0};

      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wstringop-truncation"
      strncpy(oldFolder, fileFolder, 48);
      #pragma GCC diagnostic pop

      sprintf(fileFolder, "High Priority - %s", oldFolder);
    }
    
    if (!highPriorityLogging && hplEnabled ) {
      hplEnabled = false;
      
      // Remove HP- prefix
      strcpy(fileFolder, fileFolder + 16);
    }

    // Create directory to store logs
    if (!usbFS.exists(fileFolder)) {
      Serial.print("log:Creating directory ");
      Serial.println(fileFolder);
      usbFS.mkdir(fileFolder);
    }

    sprintf(fileName, "%s/%lu.csv", fileFolder, currentMillis);
    writeHeaders(fileName);
    
    Serial.print("log:Writing to ");
    Serial.println(fileName);
  }
}