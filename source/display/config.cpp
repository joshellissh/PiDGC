#include <QJsonDocument>
#include "config.h"

Config::Config() {
    reset();
}

void Config::reset() {
    blinkerSound = true;
    ppm = 10500;
    screenDimming = 20;

    configRead = false;
    lastConfigAttempt = 0;
}

void Config::deserialize(const QJsonObject &json) {
    QMutexLocker locker(&mutex);

    if (json.contains("blinkerSound"))
       blinkerSound = json["blinkerSound"].toBool();

    if (json.contains("ppm"))
       ppm = json["ppm"].toInt();

    if (json.contains("screenDimming"))
       screenDimming = json["screenDimming"].toInt();
}

QString Config::serialize() {
    QMutexLocker locker(&mutex);

    QJsonObject jobject;
    jobject["blinkerSound"] = blinkerSound;
    jobject["ppm"] = ppm;
    jobject["screenDimming"] = screenDimming;

    QJsonDocument doc( jobject );
    return doc.toJson();
}
