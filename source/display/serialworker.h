#ifndef SERIALWORKER_H
#define SERIALWORKER_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include "vehiclevalues.h"

class SerialWorker : public QObject
{
    Q_OBJECT

public:
    SerialWorker(VehicleValues &vehicle);

public slots:
    void doWork();

private:
    VehicleValues *vehicle;
    QByteArray receiveBuffer;

    QString getSerialPort();
    void handleLine(QString line);
};

#endif // SERIALWORKER_H
