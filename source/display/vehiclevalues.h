#ifndef VEHICLEVALUES_H
#define VEHICLEVALUES_H

#include <QJsonObject>
#include <QMutex>
#include <QMutexLocker>
#include <QJsonDocument>

/**
 * @brief This is NOT the right way to implement a thread safe data class in C++.
 * It works, but it is not performant due to using a single mutex and all these garbage
 * getters/setters. I just couldn't be bothered to do it the (more tedius (yay C++! :facepalm:)
 * right way given the performance this gauge cluster needs to hit and how relatively "slow"
 * this needs to be updated.
 */
class VehicleValues
{
public:
    VehicleValues();

    bool initialized = false;
    bool resetTrip = false;
    bool writePPM = false;
    int newPPM = 8000;

    void deserialize(const QJsonObject &json);
    QString serialize();
    void reset();

    void setBoost(float boost) { QMutexLocker locker(&mutex); this->boost = boost; }
    float getBoost() { QMutexLocker locker(&mutex); return this->boost; }
    void setBoostLaggingMax(float boostLaggingMax) { QMutexLocker locker(&mutex); this->boostLaggingMax = boostLaggingMax; }
    float getBoostLaggingMax() { QMutexLocker locker(&mutex); return this->boostLaggingMax; }
    void setCoolant(float coolant) { QMutexLocker locker(&mutex); this->coolant = coolant; }
    float getCoolant() { QMutexLocker locker(&mutex); return this->coolant; }
    void setFuel(float fuel) { QMutexLocker locker(&mutex); this->fuel = fuel; }
    float getFuel() { QMutexLocker locker(&mutex); return this->fuel; }
    void setGaugeLights(bool gaugeLights) { QMutexLocker locker(&mutex); this->gaugeLights = gaugeLights; }
    float getGaugeLights() { QMutexLocker locker(&mutex); return this->gaugeLights; }
    void setHighBeam(bool highBeam) { QMutexLocker locker(&mutex); this->highBeam = highBeam; }
    bool getHighBeam() { QMutexLocker locker(&mutex); return this->highBeam; }
    void setLeftBlinker(bool leftBlinker) { QMutexLocker locker(&mutex); this->leftBlinker = leftBlinker; }
    bool getLeftBlinker() { QMutexLocker locker(&mutex); return this->leftBlinker; }
    void setLowBeam(bool lowBeam) { QMutexLocker locker(&mutex); this->lowBeam = lowBeam; }
    bool getLowBeam() { QMutexLocker locker(&mutex); return this->lowBeam; }
    void setMil(bool mil) { QMutexLocker locker(&mutex); this->mil = mil; }
    bool getMil() { QMutexLocker locker(&mutex); return this->mil; }
    void setMph(float mph) { QMutexLocker locker(&mutex); this->mph = mph; }
    float getMph() { QMutexLocker locker(&mutex); return this->mph; }
    void setOdometer(float odometer) { QMutexLocker locker(&mutex); this->odometer = odometer; }
    float getOdometer() { QMutexLocker locker(&mutex); return this->odometer; }
    void setOilPressure(float oilPressure) { QMutexLocker locker(&mutex); this->oilPressure = oilPressure; }
    float getOilPressure() { QMutexLocker locker(&mutex); return this->oilPressure; }
    void setPPM(int ppm) { QMutexLocker locker(&mutex); this->ppm = ppm; }
    float getPPM() { QMutexLocker locker(&mutex); return this->ppm; }
    void setReverse(bool reverse) { QMutexLocker locker(&mutex); this->reverse = reverse; }
    bool getReverse() { QMutexLocker locker(&mutex); return this->reverse; }
    void setRightBlinker(bool rightBlinker) { QMutexLocker locker(&mutex); this->rightBlinker = rightBlinker; }
    bool getRightBlinker() { QMutexLocker locker(&mutex); return this->rightBlinker; }
    void setRpm(float rpm) { QMutexLocker locker(&mutex); this->rpm = rpm; }
    float getRpm() { QMutexLocker locker(&mutex); return this->rpm; }
    void setSerialConnected(bool serialConnected) { QMutexLocker locker(&mutex); this->serialConnected = serialConnected; }
    bool getSerialConnected() { QMutexLocker locker(&mutex); return this->serialConnected; }
    void setTripOdometer(float tripOdometer) { QMutexLocker locker(&mutex); this->tripOdometer = tripOdometer; }
    float getTripOdometer() { QMutexLocker locker(&mutex); return this->tripOdometer; }
    void setVoltage(float voltage) { QMutexLocker locker(&mutex); this->voltage = voltage; }
    float getVoltage() { QMutexLocker locker(&mutex); return this->voltage; }


private:
    mutable QMutex mutex;

    float boost;
    float boostLaggingMax;
    float coolant;
    float fuel;
    bool gaugeLights;
    bool highBeam;
    bool leftBlinker;
    bool lowBeam;
    bool mil;
    float mph;
    float odometer;
    float oilPressure;
    int ppm;
    bool reverse;
    bool rightBlinker;
    float rpm;
    bool serialConnected;
    float tripOdometer;
    float voltage;
};

#endif // VEHICLEVALUES_H
