#ifndef PATTERNBUZZER_H
#define PATTERNBUZZER_H

class PatternBuzzer {
public:
  PatternBuzzer(unsigned int pin, unsigned int pattern[], unsigned int patternSize, unsigned int onTime, unsigned int offTime, unsigned int repeatDelayMillis = 0) {
    this->pin = pin;
    this->pattern = pattern;
    this->patternSize = patternSize;
    this->onTime = onTime;
    this->offTime = offTime;
    this->repeatDelayMillis = repeatDelayMillis;
    this->lastTriggered = -repeatDelayMillis;

    playing = false;
    playState = 0;

    pinMode(pin, OUTPUT);
    digitalWrite(pin, false);
  }
  
  void loop() {
    if (playing) {      
      if (elapsed < onTime) {
        if (pattern[playState]) {
          digitalWrite(pin, true);
        }
      } else {
        digitalWrite(pin, false);
      }

      if (elapsed >= onTime + offTime) {
        elapsed = 0;
        playState++;
      }

      if (playState >= patternSize) {
        playing = false;
      }
    }
  }

  void play() {
    if (millis() - lastTriggered >= repeatDelayMillis) {
      lastTriggered = millis();
      playing = true;
      elapsed = 0;
      playState = 0;
    } 
  }

private:
  unsigned int pin, playState, patternSize, onTime, offTime, *pattern;
  bool playing;
  unsigned int repeatDelayMillis, lastTriggered;
  elapsedMillis elapsed;
};

#endif
