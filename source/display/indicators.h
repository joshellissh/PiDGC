#ifndef INDICATORS_H
#define INDICATORS_H


class Indicators
{
public:
    Indicators();

    bool battery;
    bool coolant;
    bool fuel;
    bool oil;

    bool boostLaggingMax;
    float boostLaggingMaxOpacity;
};

#endif // INDICATORS_H
