#define NO_PULSE    99999
#define LOW_FREQ    15000000  // 15000000
#define MED_FREQ    250000    // 250000
#define HI_FREQ     100000    // 100000

#include "canbus.h"
#include "pins.h"
#include "sdcard.h"
#include "values.h"

Canbus canbus;
SDCard sdCard;
Values values;

void setup() {
  // Turn on LED as a power indicator
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Initialize serial
  Serial.begin(115200);

  // Initialize SD card systems
  sdCard.initCard();

  // Configure CAN bus
  canbus.init();

  // Initialize smoothed variables
  values.fuelLevel.begin();
  values.mph.begin();

  // Set value to expiration so it will fire immediately on boot
  values.lowFrequency = LOW_FREQ;

  // Set pin modes
  pinMode(VSS, INPUT_PULLUP);
  attachInterrupt(VSS, vssInterrupt, RISING);
  pinMode(HIGH_BEAMS, INPUT);
  pinMode(LOW_BEAMS, INPUT);
  pinMode(RIGHT_BLINKER, INPUT);
  pinMode(LEFT_BLINKER, INPUT);
  pinMode(REVERSE, INPUT);
  pinMode(MIL, INPUT);
  pinMode(GAUGE_LIGHTS, INPUT);
}

void loop() {
  // Request engine RPM
  Serial.println("Requesting RPM...");
  canbus.send(0x0c);
  
  delay(1000);

  values.fuelLevel.reading(analogRead(FUEL_LEVEL));
  values.gaugeLights = digitalRead(GAUGE_LIGHTS);
  values.highBeams = digitalRead(HIGH_BEAMS);
  values.leftBlinker = digitalRead(LEFT_BLINKER);
  values.lowBeams = digitalRead(LOW_BEAMS);
  values.mil = digitalRead(MIL);
  values.reverse = digitalRead(REVERSE);
  values.rightBlinker = digitalRead(RIGHT_BLINKER);
  values.voltage = analogRead(VOLTAGE);
}

void vssInterrupt() {
  unsigned long currentPulse = micros();
  
  if (values.vssLastPulse > currentPulse) {
    values.vssLastPulse = currentPulse;
    values.vssPulseSeparation = NO_PULSE;
  } else {
    values.vssPulseSeparation = currentPulse - values.vssLastPulse;
    values.vssLastPulse = currentPulse;
  }

  values.vssPulseCounter++;
}
