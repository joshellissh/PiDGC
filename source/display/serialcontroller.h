#ifndef SERIALCONTROLLER_H
#define SERIALCONTROLLER_H

#include <QObject>
#include <QThread>
#include "vehiclevalues.h"
#include "serialworker.h"

class SerialController : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    SerialController(VehicleValues &vehicle, Indicators &indicators) {
        SerialWorker *worker = new SerialWorker(vehicle, indicators);
        worker->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &SerialController::start, worker, &SerialWorker::doWork);
        workerThread.start();
    }
    ~SerialController() {
        workerThread.quit();
        workerThread.wait();
    }
public slots:
    void handleResults(const QString &) {
        // derp
    }
signals:
    void start();
};

#endif // SERIALCONTROLLER_H
