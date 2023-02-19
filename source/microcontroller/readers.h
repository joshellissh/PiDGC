#include "pins.h"

// Reads battery voltage (0-15)
float readBattery() {
  float Vin = ((float)analogRead(IGNI) / 1024.0) * 3.3;
  
  return (127.0 * Vin) / 27.0;
}

int readHighBeam() {
  return digitalRead(HIBM);
}

int readLowBeam() {
  return digitalRead(LOBM);
}

int readReverse() {
  return digitalRead(REV);
}

int readRightIndicator() {
  return digitalRead(RIGHT);
}

int readLeftIndicator() {
  return digitalRead(LEFT);
}

int readMIL() {
  return digitalRead(MIL);
}

int readGaugeLights() {
  return digitalRead(GAUGE_LIGHTS);
}

// Read exponential fuel level
float readFuelLevel() {
  float fuelRaw = (float)analogRead(FUEL);

  if (fuelRaw < 25.0)
    return 0.0;
    
  float fuelProcessed = 1.55397 * pow(0.995521, fuelRaw);
  return min(fuelProcessed, 1.0);
}
