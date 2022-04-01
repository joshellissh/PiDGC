#ifndef COMMSSERVER_H
#define COMMSSERVER_H

#include <QTcpServer>
#include "vehiclevalues.h"

class CommsServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit CommsServer(VehicleValues &vehicle, QObject *parent = 0);
    void startServer();
signals:

public slots:

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    VehicleValues *vehicle;
};

#endif // COMMSSERVER_H
