#ifndef VEHICLEVALUES_H
#define VEHICLEVALUES_H

#include "movingAvg.h"

class Values {
public:
  float boostPressure;
  float coolantTemp;
  movingAvg fuelLevel;
  bool gaugeLights;
  bool highBeams;
  bool leftBlinker;
  bool lowBeams;
  bool mil;
  movingAvg mph;
  float oilPressure;
  float odometer;
  bool reverse;
  bool rightBlinker;
  int rpm;
  float tripOdometer;
  float voltage;

  // Vehicle speed sensor
  volatile unsigned long vssLastPulse;
  volatile unsigned long vssPulseSeparation;
  volatile unsigned int vssPulseCounter;

  // Serial message frequency trackers
  elapsedMicros lowFrequency;
  elapsedMicros mediumFrequency;
  elapsedMicros highFrequency;
  
  Values() : fuelLevel(20), mph(20) {
    vssLastPulse = 0;
    vssPulseSeparation = 0;
    vssPulseCounter = 0;
  }
};

#endif
