#ifndef INDICATORS_H
#define INDICATORS_H

class Indicators {
public:
  bool battery;
  bool coolant;
  bool fuel;
  bool oil;

  void update(BuzzerSound &milSound) {
    // Oil pressure indicator
    if (values.oilPressure <= 20) {
      oil = true;
      playMil(milSound);
    } else {
      oil = false;
    }
  
    // Battery indicator
    if (values.voltage.get() <= 11.75f || values.voltage.get() > 15.0f) {
      battery = true;
      playMil(milSound);
    } else {
      battery = false;
    }
  
    // Coolant indicator
    if (values.coolantTemp >= 220.0f) {
      coolant = true;
      playMil(milSound);
    } else {
      coolant = false;
    }
  
    // Fuel indicator
    if (values.fuelLevel.get() <= 0.25f) {
      fuel = true;
      playMil(milSound);
    } else {
      fuel = false;
    }
  }

private:
  long int lastMil = 0;

  void playMil(BuzzerSound &milSound) {
    // Prevent MIL from playing due to indicators more often than once every 60s
    // Prevent MIL from playing if engine isn't started
    if (millis() - lastMil > 60000 && values.rpm > 0) {
      milSound.play();
      lastMil = millis();
    }
  }
};

#endif
