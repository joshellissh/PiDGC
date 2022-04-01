#include <QCoreApplication>
#include <QTcpSocket>
#include <QThread>
#include "commsserver.h"
#include <QCoreApplication>
#include "commsthread.h"

CommsServer::CommsServer(VehicleValues &vehicle, QObject *parent) : QTcpServer(parent)
{
    this->vehicle = &vehicle;
}

void CommsServer::startServer()
{
    int port = 4025;

    if(!this->listen(QHostAddress::Any, port)) {
        qDebug() << "Could not start server";
        QCoreApplication::exit(1);
    } else {
        qDebug() << "Listening on port" << port << "...";
    }
}

void CommsServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << socketDescriptor << " Connecting...";

    // Start new thread
    CommsThread *thread = new CommsThread(socketDescriptor, *vehicle, this);

    // Connect signal/slot
    // Once a thread is not needed, it will be deleted later
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
