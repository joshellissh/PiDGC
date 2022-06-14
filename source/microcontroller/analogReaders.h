#include "pins.h"

// Reads battery voltage (0-15)
float readVoltage() {
  float Vin = ((float)analogRead(VOLTAGE) / 1024.0) * 3.3;
  
  return (127.0 * Vin) / 27.0;
}

int readHighBeams() {
  return digitalRead(HIGH_BEAMS);
}

int readLowBeams() {
  return digitalRead(LOW_BEAMS);
}

int readRightBlinker() {
  return digitalRead(RIGHT_BLINKER);
}

int readLeftBlinker() {
  return digitalRead(LEFT_BLINKER);
}

int readMIL() {
  return digitalRead(MIL);
}

int readGaugeLights() {
  return digitalRead(GAUGE_LIGHTS);
}

// Read exponential fuel level
float readFuelLevel() {
  float fuelRaw = (float)analogRead(FUEL_LEVEL);

  if (fuelRaw < 25.0)
    return 0.0;
    
  float fuelProcessed = 1.55397 * pow(0.995521, fuelRaw);
  return min(fuelProcessed, 1.0);
}
