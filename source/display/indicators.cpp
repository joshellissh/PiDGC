#include "indicators.h"

Indicators::Indicators() {
    oil = false;
    battery = false;
    fuel = false;
    coolant = false;

    boostLaggingMax = false;
    boostLaggingMaxOpacity = 0.0f;
}
