#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTime>
#include "serialworker.h"

SerialWorker::SerialWorker(VehicleValues &vehicle, Indicators &indicators) {
    this->vehicle = &vehicle;
    this->indicators = &indicators;
}

void SerialWorker::doWork() {
    qDebug() << "Started SerialWorker";

    // Run indefinitely
    while(!vehicle->exiting) {
        QString portName = getSerialPort();

        if (portName == NULL) {
            qDebug() << "No serial ports found.";
            QThread::sleep(1);
            continue;
        }

        qDebug() << "Connecting to port" << portName;
        QSerialPort serialPort;
        serialPort.setPortName(portName);
        serialPort.setBaudRate(QSerialPort::Baud115200);

        if (!serialPort.open(QSerialPort::ReadWrite)) {
            qDebug() << "Unable to open serial port.";
            QThread::sleep(1);
            continue;
        } else {
            qDebug() << "Connected to" << portName;
        }

        vehicle->lastSerialRead = QTime::currentTime().msecsSinceStartOfDay();
        serialPort.clear();

        while(serialPort.isOpen() && !vehicle->exiting && QTime::currentTime().msecsSinceStartOfDay() - vehicle->lastSerialRead < 5000) {
            vehicle->setSerialConnected(true);

            if (vehicle->initLoopFinished) {
                // Request odometer values if necessary
                if (!vehicle->odometerRead && QTime::currentTime().msecsSinceStartOfDay() - vehicle->lastOdometerAttempt > 1000) {
                    serialPort.write("so\n");
                    serialPort.waitForBytesWritten(100);
                    vehicle->lastOdometerAttempt = QTime::currentTime().msecsSinceStartOfDay();
                    qDebug() << "Requested odometer values.";
                }

                // Request config values if necessary
                if (!vehicle->config.configRead && QTime::currentTime().msecsSinceStartOfDay() - vehicle->config.lastConfigAttempt > 1000) {
                    serialPort.write("sconfig\n");
                    serialPort.waitForBytesWritten(100);
                    vehicle->config.lastConfigAttempt = QTime::currentTime().msecsSinceStartOfDay();
                    qDebug() << "Requested configuration values.";
                }
            }

            // Write if anything is queued
            {
                QMutexLocker locker(&vehicle->queueMutex);
                while (!vehicle->serialWriteQueue.isEmpty()) {
                    QString toWrite = vehicle->serialWriteQueue.dequeue();

                    if (!toWrite.endsWith('\n')) {
                        toWrite.append('\n');

                    }
                    serialPort.write(toWrite.toUtf8());
                }
                locker.unlock();
            }

            // Read if anything is available
            if (serialPort.waitForReadyRead(1)){
                receiveBuffer += serialPort.readAll().replace("\r", "");

                if (receiveBuffer.contains('\n')) {
                    int lastNewline = receiveBuffer.lastIndexOf('\n');
                    QString processBuffer = QString(receiveBuffer).left(lastNewline);
                    receiveBuffer.remove(0, lastNewline + 1);

                    // Break buffer into line list
                    QStringList list = QString(processBuffer).split("\n");

                    for (int i=0; i<list.length(); i++)
                        handleLine(list[i]);
                }

                vehicle->lastSerialRead = QTime::currentTime().msecsSinceStartOfDay();
            }
        }

        vehicle->reset();
        serialPort.close();
        vehicle->setSerialConnected(false);
    }
}

void SerialWorker::handleLine(QString line) {
    QStringList values = line.split(":");

    if (!vehicle->initLoopFinished)
        return;

    if (values[0] == "rpm") {
        vehicle->setRpm(values[1].toInt());
    } else if (values[0] == "boost") {
        vehicle->setBoost(values[1].toFloat());
    } else if (values[0] == "mph") {
        vehicle->setMph(values[1].toInt());
    } else if (values[0] == "oil") {
        vehicle->setOilPressure(values[1].toFloat());
    } else if (values[0] == "voltage") {
        vehicle->setVoltage(values[1].toFloat());
    } else if (values[0] == "fuel") {
        vehicle->setFuel(values[1].toFloat());
    } else if (values[0] == "hi") {
        vehicle->setHighBeam(values[1].toInt());
    } else if (values[0] == "left") {
        vehicle->setLeftBlinker(values[1].toInt());
    } else if (values[0] == "low") {
        vehicle->setLowBeam(values[1].toInt());
    } else if (values[0] == "right") {
        vehicle->setRightBlinker(values[1].toInt());
    } else if (values[0] == "mil") {
        vehicle->setMil(values[1].toInt());
    } else if (values[0] == "glite") {
        vehicle->setGaugeLights(values[1].toInt());
    } else if (values[0] == "mph") {
        vehicle->setMph(values[1].toInt());
    } else if (values[0] == "coolant") {
        vehicle->setCoolant(values[1].toFloat());
    } else if (values[0] == "log") {
        qDebug() << qPrintable(line.replace("log:", "uC:"));
    } else if (values[0] == "odo") {
        QStringList parts = values[1].split(",");

        if (parts.length() != 2) {
            qDebug() << "Invalid length for odometer return.";
            return;
        }

        vehicle->setTripOdometer(parts[0].toFloat());
        vehicle->setOdometer(parts[1].toFloat());
        vehicle->odometerRead = true;
    } else if (values[0] == "indicators") {
        QStringList parts = values[1].split(",");

        if (parts.length() != 4) {
            qDebug() << "Invalid length for indicators return.";
            return;
        }

        indicators->coolant = parts[0].toInt();
        indicators->fuel = parts[1].toInt();
        indicators->oil = parts[2].toInt();
        indicators->battery = parts[3].toInt();
    } else if (values[0] == "config") {
        QStringList parts = values[1].split(",");

        if (parts.length() != 3) {
            qDebug() << "Invalid length for config return.";
            return;
        }

        vehicle->config.setPPM(parts[0].toInt());
        vehicle->config.setBlinkerSound(parts[1].toInt());
        vehicle->config.setScreenDimming(parts[2].toInt());
        vehicle->config.configRead = true;
    }

}

QString SerialWorker::getSerialPort() {
    QSerialPortInfo info;
    QList<QSerialPortInfo> ports = info.availablePorts();

    if (ports.size() == 0)
        return NULL;
    else
        return ports[0].portName();
}
