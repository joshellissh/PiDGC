#ifndef VEHICLEVALUES_H
#define VEHICLEVALUES_H

#include <Smoothed.h>

class Values {
public:
  float barometricPressure;
  bool blinkerSound;
  float boostPressure;
  int coolantTemp;
  float footPedalPosition;
  Smoothed<float> fuelLevel;
  float fuelRailPressure;
  bool gaugeLights;
  bool highBeams;
  int intakeAirTemp;
  bool leftBlinker;
  float longTermFuelTrimB1;
  bool lowBeams;
  float manifoldAbsolutePressure;
  bool mil;
  Smoothed<int> mph;
  float oilPressure;
  float odometer;
  float oxySensor1AFER;
  int ppm;
  bool rightBlinker;
  int rpm;
  int screenDimming;
  float shortTermFuelTrimB1;
  float timingAdvance;
  float throttleInletPressure;
  float throttlePosition;
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

    barometricPressure = 14.1f;
    boostPressure = 0.0f;
    coolantTemp = 0;
    oilPressure = 0.0f;
    rpm = 0;
  }
};

#endif
