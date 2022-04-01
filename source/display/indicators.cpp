#include "indicators.h"

Indicators::Indicators()
{
    left = false;
    right = false;
    lowBeam = false;
    highBeam = false;
    mil = false;
    oil = false;
    battery = false;
    fuel = false;
    coolant = false;
    boostLaggingMax = false;
    boostLaggingMaxOpacity = 0.0f;
    shiftLight = false;
    serialConnected = false;
    gaugeLights = false;
}
