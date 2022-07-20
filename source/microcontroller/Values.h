#ifndef VEHICLEVALUES_H
#define VEHICLEVALUES_H

#include <Smoothed.h>

class Values {
public:
  float barometricPressure;
  bool blinkerSound;
  float boostPressure;
  int coolantTemp;
  Smoothed<float> fuelLevel;
  bool gaugeLights;
  bool highBeams;
  bool leftBlinker;
  bool lowBeams;
  bool mil;
  Smoothed<int> mph;
  float oilPressure;
  float odometer;
  int ppm;
  bool rightBlinker;
  int rpm;
  int screenDimming;
  float tripOdometer;
  Smoothed<float> voltage;

  // Vehicle speed sensor
  volatile unsigned long vssLastPulse;
  volatile unsigned long vssPulseSeparation;
  volatile unsigned int vssPulseCounter;

  // Serial message frequency trackers
  elapsedMicros lowFrequency;
  elapsedMicros mediumFrequency;
  elapsedMicros highFrequency;
  
  Values() {
    vssLastPulse = 0;
    vssPulseSeparation = 0;
    vssPulseCounter = 0;
    ppm = 10500;
  }
};

#endif
