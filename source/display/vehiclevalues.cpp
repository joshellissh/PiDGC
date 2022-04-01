#include "vehiclevalues.h"

VehicleValues::VehicleValues()
{
    reset();
}

void VehicleValues::reset() {
    rpm = 0.0f;
    mph = 0.0f;
    coolant = 170.0f;
    fuel = 0.0f;
    gaugeLights = false;
    boost = 0.0f;
    voltage = 0.0f;
    odometer = 0.0f;
    tripOdometer = 0.0f;
    boostLaggingMax = 0.0f;
    reverse = false;
    oilPressure = 0.0f;
    leftBlinker = false;
    rightBlinker = false;
    lowBeam = false;
    highBeam = false;
    mil = false;
    serialConnected = false;
    ppm = 8000;
}

void VehicleValues::deserialize(const QJsonObject &json) {
    QMutexLocker locker(&mutex);

//    QJsonDocument doc(json);
//    qDebug() << doc.toJson(QJsonDocument::Compact);

    if (json.contains("rpm") && json["rpm"].isDouble())
       rpm = json["rpm"].toDouble();

    if (json.contains("mph") && json["mph"].isDouble())
       mph = json["mph"].toDouble();

    if (json.contains("coolant") && json["coolant"].isDouble())
       coolant = json["coolant"].toDouble();

    if (json.contains("fuel") && json["fuel"].isDouble())
       fuel = json["fuel"].toDouble();

    if (json.contains("gaugeLights") && json["gaugeLights"].isBool())
       gaugeLights = json["gaugeLights"].toBool();

    if (json.contains("boost") && json["boost"].isDouble())
       boost = json["boost"].toDouble();

    if (json.contains("battery") && json["battery"].isDouble())
       voltage = json["battery"].toDouble();

    if (json.contains("odometer") && json["odometer"].isDouble())
       odometer = json["odometer"].toDouble();

    if (json.contains("tripOdometer") && json["tripOdometer"].isDouble())
       tripOdometer = json["tripOdometer"].toDouble();

    if (json.contains("reverse") && json["reverse"].isBool())
       reverse = json["reverse"].toBool();

    if (json.contains("oilPressure") && json["oilPressure"].isDouble())
       oilPressure = json["oilPressure"].toDouble();

    if (json.contains("vssPulsesPerMile"))
       ppm = json["vssPulsesPerMile"].toInt();

    if (json.contains("left") && json["left"].isBool())
       leftBlinker = json["left"].toBool();

    if (json.contains("right") && json["right"].isBool())
       rightBlinker = json["right"].toBool();

    if (json.contains("lowBeam") && json["lowBeam"].isBool())
       lowBeam = json["lowBeam"].toBool();

    if (json.contains("highBeam") && json["highBeam"].isBool())
       highBeam = json["highBeam"].toBool();

    if (json.contains("mil") && json["mil"].isBool())
       mil = json["mil"].toBool();

    if (json.contains("serialConnected") && json["serialConnected"].isBool())
       serialConnected = json["serialConnected"].toBool();
}

QString VehicleValues::serialize() {
    QMutexLocker locker(&mutex);

    QJsonObject jobject;
    jobject["rpm"] = rpm;
    jobject["mph"] = mph;
    jobject["coolant"] = coolant;
    jobject["fuel"] = fuel;
    jobject["gaugeLights"] = gaugeLights;
    jobject["boost"] = boost;
    jobject["voltage"] = voltage;
    jobject["odometer"] = odometer;
    jobject["tripOdometer"] = tripOdometer;
    jobject["reverse"] = reverse;
    jobject["oilPressure"] = oilPressure;
    jobject["leftBlinker"] = leftBlinker;
    jobject["rightBlinker"] = rightBlinker;
    jobject["lowBeam"] = lowBeam;
    jobject["highBeam"] = highBeam;
    jobject["mil"] = mil;
    jobject["serialConnected"] = serialConnected;
    jobject["vssPulsesPerMile"] = ppm;

    QJsonDocument doc( jobject );
    return doc.toJson();
}
