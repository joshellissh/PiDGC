#ifndef INDICATORS_H
#define INDICATORS_H


class Indicators
{
public:
    Indicators();

    bool left;
    bool right;
    bool lowBeam;
    bool highBeam;
    bool mil;
    bool oil;
    bool battery;
    bool fuel;
    bool coolant;
    bool boostLaggingMax;
    float boostLaggingMaxOpacity;
    bool shiftLight;
    bool serialConnected;
    bool gaugeLights;
};

#endif // INDICATORS_H
