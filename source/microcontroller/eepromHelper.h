#include <EEPROM.h>
#include "types.h"

#define ODOMETER_WHOLE    0
#define ODOMETER_TENTHS   270
#define TRIP_WHOLE        540
#define TRIP_TENTHS       810
#define PULSES_PER_MILE   1040
#define BLINKER_SOUND     1045
#define CHIME_SOUND       1050
#define SCREEN_DIMMING    1055

float odometer = 0.0f;
float tripOdometer = 0.0f;

int wholePart(float n) {
  return (int)n;
}

byte tenthsPart(float n) {
  return int(n * 10.0f) % 10;
}

float fromParts(int whole, int tenths) {
  return (float)whole + ((float)tenths / 10.0f);
}

float readMileage(OdoType odoType) {
  int whole, tenths;
  
  if (odoType == REGULAR) {
    EEPROM.get(ODOMETER_WHOLE, whole);
    EEPROM.get(ODOMETER_TENTHS, tenths);
  } else {
    EEPROM.get(TRIP_WHOLE, whole);
    EEPROM.get(TRIP_TENTHS, tenths);
  }

  return fromParts(whole, tenths);
}

void writeMileage(OdoType odoType, float mileage) {
  int whole = wholePart(mileage);
  int tenths = tenthsPart(mileage);
  
  if (odoType == REGULAR) {
    EEPROM.put(ODOMETER_WHOLE, whole);
    EEPROM.put(ODOMETER_TENTHS, tenths);
  } else {
    EEPROM.put(TRIP_WHOLE, whole);
    EEPROM.put(TRIP_TENTHS, tenths);
  }
}

void sendOdometerValues() {
  float tripOdometer = readMileage(TRIP);
  float odometer = readMileage(REGULAR);

  char output[512] = {0};
  sprintf(output, "odo:%f,%f", tripOdometer, odometer);
  Serial.println(output);
  
  sprintf(
    output, 
    "log:SERIAL_LOG-Odometer values requested. Returning (trip %f / odometer %f)",
    tripOdometer,
    odometer
  );
  Serial.println(output);
}

int readValue(int defaultVal, int valueAddress) {
  int val = defaultVal;
  EEPROM.get(valueAddress, val);
  return val;
}

void writeValue(int value, int valueAddress) {
  EEPROM.put(valueAddress, value);
}

void sendConfig() {
  int ppm = readValue(8000, PULSES_PER_MILE);
  bool blinkerSound = readValue(true, BLINKER_SOUND);
  bool chimeSound = readValue(true, CHIME_SOUND);
  int screenDimming = readValue(20, SCREEN_DIMMING);

  char output[512] = {0};
  sprintf(output, "config:%d,%d,%d,%d", ppm, blinkerSound, chimeSound, screenDimming);
  Serial.println(output);
  
  sprintf(
    output, 
    "log:SERIAL_LOG-Config value requested. Returning %d, %d, %d, %d",
    ppm,
    blinkerSound,
    chimeSound,
    screenDimming
  );
  Serial.println(output);
}
