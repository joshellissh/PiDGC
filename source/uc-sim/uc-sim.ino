#include <math.h>
#include "values.h"

Values values;
float oscillator = 0.0f;

void setup() {
   // Turn on LED as a power indicator
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Initialize serial
  Serial.begin(115200);
}

void loop() {
  values.boostPressure = 15.0f;

  oscillator += 0.05;

  values.boostPressure = 15.0f + sin(oscillator)*15.0f;
  values.coolantTemp = 200.0f + sin(oscillator)*50.0f;
  values.fuelLevel = 0.5f + sin(oscillator)*0.5f;
  values.gaugeLights = false;//(bool)round(sin(oscillator));
  values.highBeams = (bool)round(sin(oscillator));
  values.leftBlinker = (bool)round(sin(oscillator));
  values.lowBeams = (bool)round(sin(oscillator));
  values.mil = (bool)round(sin(oscillator));
  values.mph = 80.0f + sin(oscillator)*80.0f;
  values.oilPressure = 50.0f + sin(oscillator)*40.0f;
  values.odometer = 1500.0f + sin(oscillator)*1500.0f;
  values.reverse = (bool)round(sin(oscillator));
  values.rightBlinker = (bool)round(sin(oscillator));
  values.rpm = 4000.0f + sin(oscillator)*4000.0f;
  values.tripOdometer = 1500.0f + sin(oscillator)*1500.0f;
  values.voltage = 13.0f + sin(oscillator)*2.0f;
  
  // Send rpm & boost
  char output[512] = {0};
  sprintf(output, "rpm:%d\nboost:%f\ncoolant:%d", values.rpm, values.boostPressure, values.coolantTemp);
  Serial.println(output);

  sprintf(
    output, 
    "voltage:%f\nfuel:%f\nhi:%d\nleft:%d\nlo:%d\nrev:%d\nright:%d\nmil:%d\nglite:%d\nmph:%d\noil:%f",
    values.voltage,
    values.fuelLevel,
    values.highBeams,
    values.leftBlinker,
    values.lowBeams,
    values.reverse,
    values.rightBlinker,
    values.mil,
    values.gaugeLights,
    values.mph,
    values.oilPressure
  );
  Serial.println(output);

  sprintf(output, "odo:%f,%f", values.tripOdometer, values.odometer);
  Serial.println(output);

  delay(100);
}
