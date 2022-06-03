#define NO_PULSE    99999
#define LOW_FREQ    5000000   // 500ms
#define MED_FREQ    250000    // 250ms
#define HI_FREQ     100000    // 100ms

#include "pins.h"
#include "sdcard.h"
#include "values.h"
#include "serialreader.h"
#include "serialComms.h"
#include "analogReaders.h"
#include "canbus.h"

SDCard sdCard;
Values values;
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;

void setup() {
  // Turn on LED as a power indicator
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Initialize serial
  Serial.begin(115200);

  // Initialize SD card systems
  sdCard.initCard();

  // Configure CAN bus
  canInit();

  // Initialize smoothed variables
  values.fuelLevel.begin(SMOOTHED_AVERAGE, 100);
  values.mph.begin(SMOOTHED_AVERAGE, 40);
  values.voltage.begin(SMOOTHED_AVERAGE, 10);

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

  // Load initial values from files
  values.tripOdometer = sdCard.readFloat(TRIP_FILE, 0.0f);
  values.odometer = sdCard.readFloat(ODOMETER_FILE, 0.0f);
  values.ppm = sdCard.readInt(PPM_FILE, 10500);
  values.blinkerSound = sdCard.readBool(BLINKER_FILE, true);
  values.chimeSound = sdCard.readBool(CHIME_FILE, true);
  values.screenDimming = sdCard.readInt(DIMMING_FILE, 20);  
}

void loop() {
  handleComms(sdCard, values);
  readAnalogPins();

  // Process necessary canbus events
  can1.events();

  // High frequency updates
  if (values.highFrequency >= HI_FREQ) {
    values.highFrequency -= HI_FREQ;

    // Request engine RPM
    canSend(0x0c);

    // Request MAP
    canSend(0x0B);

    cli();
    unsigned int numPulses = values.vssPulseCounter;
    unsigned int pulseSepMicros = values.vssPulseSeparation;
    sei();
    values.vssPulseCounter = 0;
    values.vssPulseSeparation = NO_PULSE;

    // Pulses counted / pulses per mile = distance travelled.
    float distance = (float)numPulses / (float)values.ppm;

    // Update odometers
    values.odometer += distance;
    values.tripOdometer += distance;

    // Write updated odometers to file
    writeOdometers(values.tripOdometer, values.odometer);

    // Update MPH
    if (pulseSepMicros != NO_PULSE && pulseSepMicros > 0) {
        // Calculate MPH
        float oneMphInMicros = 3600000000.0f / (float)values.ppm;
        float mph = oneMphInMicros / (float)pulseSepMicros;
        values.mph.add(mph);
    } else {
        values.mph.add(0.0);
    }

    // Send rpm & boost
    char output[256] = {0};
    sprintf(output, "rpm:%d\nboost:%f", values.rpm, values.boostPressure);
    Serial.println(output);
  }

  // Medium frequency updates
  if (values.mediumFrequency >= MED_FREQ) {
    values.mediumFrequency -= MED_FREQ;

    char output[512] = {0};
    sprintf(
      output, 
      "voltage:%f\nfuel:%f\nhi:%d\nleft:%d\nlo:%d\nrev:%d\nright:%d\nmil:%d\nglite:%d\nmph:%d",
      values.voltage.get(),
      values.fuelLevel.get(),
      readHighBeams(),
      readLeftBlinker(),
      readLowBeams(),
      readReverse(),
      readRightBlinker(),
      readMIL(),
      readGaugeLights(),
      values.mph.get()
    );
    Serial.println(output);
  }

  // Low frequency updates
  if (values.lowFrequency >= LOW_FREQ) {
    values.lowFrequency -= LOW_FREQ;

    // Request coolant temp
    canSend(0x05);

    // Request Barometric Pressure
    canSend(0x33);

    char output[256] = {0};
    sprintf(output, "coolant:%d", values.coolantTemp);
    Serial.println(output);
  }
}

void readAnalogPins() {
  values.fuelLevel.add(readFuelLevel());
  values.voltage.add(readVoltage());
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

void writeOdometers(float trip, float odometer) {
  char currentTrip[10] = {0};
  char currentOdometer[10] = {0};
  char writeTrip[10] = {0};
  char writeOdometer[10] = {0};
  
  dtostrf(sdCard.readFloat(TRIP_FILE, 0.0f), 0, 1, currentTrip);
  dtostrf(sdCard.readFloat(ODOMETER_FILE, 0.0f), 0, 1, currentOdometer);
  dtostrf(trip, 0, 1, writeTrip);
  dtostrf(odometer, 0, 1, writeOdometer);

  // Only write out value if it has changed by more than a 10th of a mile
  if (trip != 0.0f && strcmp(currentTrip, writeTrip) != 0)
    sdCard.writeValue(TRIP_FILE, trip);

  // Only write out value if it has changed by more than a 10th of a mile
  if (odometer != 0.0f && strcmp(currentOdometer, writeOdometer) != 0)
    sdCard.writeValue(ODOMETER_FILE, odometer);
}
