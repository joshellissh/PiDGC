#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <math.h>
#include <variables.h>
#include "drawing.h"

inline float boostToAngle(float boost) {
    boost = min(max(boost, 0.0f), MAX_BOOST);
    float trueAngle = (boost / MAX_BOOST) * 247.0f;
    return trueAngle - 34.5f;
}

inline float fuelToAngle(float fuel) {
    fuel = min(max(fuel, 0.0f), 1.0f);
    float trueAngle = fuel * 247.0f;
    return trueAngle - 34.5f;
}

inline float rpmToAngle(float rpm) {
    // Limit to usable range
    rpm = min(max(rpm, 0.0f), MAX_RPM);
    float trueAngle = (rpm / MAX_RPM) * 269.0f;
    return trueAngle - 44.5f;
}

inline float mphToAngle(float mph) {
    // Limit to usable range
    mph = min(max(mph, 0.0f), MAX_MPH);
    float trueAngle = (mph / MAX_MPH) * 269.0f;
    return trueAngle - 44.5f;
}

inline float temperatureToAngle(float temp) {
    // Limit to usable range
    temp = min(max(temp, 170.0f), 270.0f);
    float trueAngle = ((270.0f - temp) / 100.0f) * 50.0f;
    return trueAngle - 115.0f;
}

inline float pressureToAngle(float pressure) {
    float range = 80.0f;
    pressure = min(max(pressure, 10.0f), 90.0f) - 10.0f;
    pressure = pressure / range;
    float trueAngle = (1.0f - pressure) * 50.0f;
    return trueAngle - 115.0f;
}

#endif // CONVERTERS_H
