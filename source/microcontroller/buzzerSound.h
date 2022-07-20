#ifndef BUZZERSOUND_H
#define BUZZERSOUND_H

class BuzzerSound {
public:
  BuzzerSound(int pin, int durationMillis, int repeatDelayMillis = 0) {
    this->pin = pin;
    this->durationMillis = durationMillis;
    this->repeatDelayMillis = repeatDelayMillis;
    this->lastTriggered = 0;

    playing = false;

    pinMode(pin, OUTPUT);
    digitalWrite(pin, false);
  }
  
  void loop() {
    if (playing && elapsed >= durationMillis) {
      playing = false;
      elapsed = 0;
      digitalWrite(pin, false);
    }
  }

  void play() {
    if (millis() - lastTriggered >= repeatDelayMillis) {
      lastTriggered = millis();
      playing = true;
      elapsed = 0;
      digitalWrite(pin, true);
    }
  }

private:
  int pin;
  bool playing;
  unsigned int durationMillis, repeatDelayMillis, lastTriggered;
  elapsedMillis elapsed;
};

#endif
