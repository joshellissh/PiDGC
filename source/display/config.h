#ifndef CONFIG_H
#define CONFIG_H

#include <QMutex>
#include <QJsonObject>


class Config
{
public:
    bool configRead = false;
    int lastConfigAttempt = 0;

    Config();

    void setBlinkerSound(bool blinkerSound) { QMutexLocker locker(&mutex); this->blinkerSound = blinkerSound; }
    float getBlinkerSound() { QMutexLocker locker(&mutex); return this->blinkerSound; }

    void setPPM(int ppm) { QMutexLocker locker(&mutex); this->ppm = ppm; }
    float getPPM() { QMutexLocker locker(&mutex); return this->ppm; }

    void setScreenDimming(int screenDimming) { QMutexLocker locker(&mutex); this->screenDimming = screenDimming; }
    float getScreenDimming() { QMutexLocker locker(&mutex); return this->screenDimming; }

    void deserialize(const QJsonObject &json);
    QString serialize();
    void reset();

private:
    mutable QMutex mutex;

    bool blinkerSound;
    int ppm;
    int screenDimming;
};

#endif // CONFIG_H
