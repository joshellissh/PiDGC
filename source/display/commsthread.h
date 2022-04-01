#ifndef COMMSTHREAD_H
#define COMMSTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>
#include "variables.h"
#include "vehiclevalues.h"

class CommsThread : public QThread
{
    Q_OBJECT
public:
    explicit CommsThread(qintptr ID, VehicleValues &vehicle, QObject *parent = 0);
    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();
    void sendInputs();

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
    VehicleValues *vehicle;
    QTimer *timer;
};

#endif // COMMSTHREAD_H
