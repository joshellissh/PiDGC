#ifndef BUZZERSOUND_H
#define BUZZERSOUND_H

class BuzzerSound {
public:
  BuzzerSound(int pin, int durationMillis) {
    this->pin = pin;
    this->durationMillis = durationMillis;

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
    playing = true;
    elapsed = 0;
    digitalWrite(pin, true);
  }

private:
  int pin;
  bool playing;
  unsigned int durationMillis;
  elapsedMillis elapsed;
};

#endif
