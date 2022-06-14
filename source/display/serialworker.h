#ifndef SERIALWORKER_H
#define SERIALWORKER_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include "vehiclevalues.h"
#include "indicators.h"

class SerialWorker : public QObject
{
    Q_OBJECT

public:
    SerialWorker(VehicleValues &vehicle, Indicators &indicators);

public slots:
    void doWork();

private:
    VehicleValues *vehicle;
    Indicators *indicators;
    QByteArray receiveBuffer;

    QString getSerialPort();
    void handleLine(QString line);
};

#endif // SERIALWORKER_H
