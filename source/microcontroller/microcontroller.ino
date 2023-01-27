#define NO_PULSE    99999
#define LOW_FREQ    5000000   // 5s
#define MED_FREQ    250000    // 250ms
#define HI_FREQ     100000    // 100ms

#include "pins.h"
#include "sdcard.h"
#include "values.h"
#include "serialreader.h"
#include "serialComms.h"
#include "analogReaders.h"
#include "canbus.h"
#include "buzzerSound.h"
#include "patternBuzzer.h"
#include "indicators.h"

SDCard sdCard;
Values values;
Indicators indicators;
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;
BuzzerSound blinkerSound(BUZZER, 50);
BuzzerSound milSound(BUZZER, 3000, 30000);
PatternBuzzer fuelBuzzer(BUZZER, (unsigned int []){1,1,1,1,0,1,1,1,1}, 9, 250, 750, 600000);
PatternBuzzer gaugeLiteBuzzer(BUZZER, (unsigned int []){1,1,0}, 3, 250, 750, 3000);

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
  values.mph.begin(SMOOTHED_AVERAGE, 20);
  values.voltage.begin(SMOOTHED_AVERAGE, 20);

  // Set value to expiration so it will fire immediately on boot
  values.lowFrequency = LOW_FREQ;

  // Set pin modes
  pinMode(VSS, INPUT_PULLUP);
  attachInterrupt(VSS, vssInterrupt, RISING);
  pinMode(HIGH_BEAMS, INPUT);
  pinMode(LOW_BEAMS, INPUT);
  pinMode(RIGHT_BLINKER, INPUT);
  pinMode(LEFT_BLINKER, INPUT);
  pinMode(MIL, INPUT_PULLDOWN);
  pinMode(GAUGE_LIGHTS, INPUT);

  // Load initial values from files
  values.tripOdometer = sdCard.readFloat(TRIP_FILE, 0.0f);
  values.odometer = sdCard.readFloat(ODOMETER_FILE, 0.0f);
  values.ppm = sdCard.readInt(PPM_FILE, 10500);
  values.blinkerSound = sdCard.readBool(BLINKER_FILE, true);
  values.screenDimming = sdCard.readInt(DIMMING_FILE, 20); 
}

void loop() {
  handleComms(sdCard, values);
  indicators.update(milSound);

  // Process necessary canbus events
  can1.events();

  // process buzzer sounds
  blinkerSound.loop();
  milSound.loop();
  fuelBuzzer.loop();
  gaugeLiteBuzzer.loop();

  // High frequency updates
  if (values.highFrequency >= HI_FREQ) {
    values.highFrequency -= HI_FREQ;

    // Request engine RPM
    canSendOBD2(0x0c);

    // Request MAP
    canSendOBD2(0x0B);

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

    canSendSpeedMPH(25);

    // Send rpm & boost
    char output[256] = {0};
    sprintf(output, "rpm:%d\nboost:%f", values.rpm, values.boostPressure);
    Serial.println(output);
  }

  // Medium frequency updates
  if (values.mediumFrequency >= MED_FREQ) {
    values.mediumFrequency -= MED_FREQ;

    readAnalogPins();

    bool prevLeftBlinker = values.leftBlinker;
    bool prevRightBlinker = values.rightBlinker;
    bool prevMil = values.mil;

    values.leftBlinker = readLeftBlinker();
    values.rightBlinker = readRightBlinker();
    values.mil = readMIL();

    if (values.blinkerSound) {
      if ((!prevLeftBlinker && values.leftBlinker) || (!prevRightBlinker && values.rightBlinker)) {
        blinkerSound.play();
      }
    }

    if (!prevMil && values.mil) {
      milSound.play();
    }

    if (readGaugeLights() == true && values.rpm <= 250) {
      gaugeLiteBuzzer.play();
    }

    char output[512] = {0};
    sprintf(
      output, 
      "voltage:%f\nfuel:%f\nhi:%d\nleft:%d\nlow:%d\nright:%d\nmil:%d\nglite:%d\nmph:%d\noil:%f",
      values.voltage.get(),
      values.fuelLevel.get(),
      readHighBeams(),
      values.leftBlinker,
      readLowBeams(),
      values.rightBlinker,
      values.mil,
      readGaugeLights(),
      values.mph.get(),
      values.oilPressure
    );
    Serial.println(output);

    float tripOdometer = sdCard.readFloat(TRIP_FILE, 0.0f);
    float odometer = sdCard.readFloat(ODOMETER_FILE, 0.0f);
    sprintf(output, "odo:%f,%f", tripOdometer, odometer);
    Serial.println(output);
  }

  // Low frequency updates
  if (values.lowFrequency >= LOW_FREQ) {
    values.lowFrequency -= LOW_FREQ;

    // Request coolant temp
    canSendOBD2(0x05);

    // Request Barometric Pressure
    canSendOBD2(0x33);

    char output[256] = {0};
    sprintf(output, "coolant:%d", values.coolantTemp);
    Serial.println(output);

    // Play fuel buzzer if needed
    if (values.fuelLevel.get() <= 0.125) {
      fuelBuzzer.play();
    }
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
    unsigned long pulseDelta = currentPulse - values.vssLastPulse;
    
    if (pulseDelta <= 500) {
      // Impossibly fast. Throw it out.
      return;
    }
    
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
