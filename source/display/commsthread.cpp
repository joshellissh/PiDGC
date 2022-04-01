#include "commsthread.h"
#include "vehiclevalues.h"
#include <QJsonDocument>
#include <QPlainTextEdit>

CommsThread::CommsThread(qintptr ID, VehicleValues &vehicle, QObject *parent) :
    QThread(parent)
{
    this->vehicle = &vehicle;
    this->socketDescriptor = ID;
}

void CommsThread::run() {
    qDebug() << "CommsThread started";

    socket = new QTcpSocket();

    // set the ID
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        // something's wrong, we just emit a signal
        emit error(socket->error());
        return;
    }

    // connect socket and signal
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    // Start send timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CommsThread::sendInputs);
    timer->start(100); // ~10 rps

    qDebug() << socketDescriptor << " Client connected";

    // Make thread loops
    exec();
}

void CommsThread::sendInputs()
{
    // Reset trip if necessary
    if (vehicle->resetTrip) {
        vehicle->resetTrip = false;
        socket->write("RESET_TRIP:0.0\n");
        qDebug() << "Sending RESET_TRIP from Display";
    }

    // Write new PPM if necessary
    if (vehicle->writePPM) {
        vehicle->writePPM = false;
        char write[128] = {0};
        sprintf(write, "WRITE_PPM:%d\n", vehicle->newPPM);
        socket->write(write);

        qDebug() << "Sending WRITE_PPM from Display";
    }
}

void CommsThread::readyRead()
{
    // Read packet
    QByteArray packet = socket->readLine();
    QJsonDocument loadDoc = QJsonDocument::fromJson(packet);

    if (vehicle->initialized)
        vehicle->deserialize(loadDoc.object());
}

void CommsThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";
    timer->stop();
    delete timer;
    socket->deleteLater();
    exit(0);
}
