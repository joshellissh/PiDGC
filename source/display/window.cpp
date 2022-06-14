#include "window.h"
#include <math.h>

Window::Window(VehicleValues &vehicle, Indicators &indicators)
{
    this->vehicle = &vehicle;
    this->indicators = &indicators;
    this->painter = new Painter(vehicle, indicators);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setWindowModality(Qt::ApplicationModal);
    setFocusPolicy(Qt::StrongFocus);
    setGeometry(0, 0, 1280, 480);

    GLWidget *openGL = new GLWidget(painter, this);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, openGL, &GLWidget::animate);
    timer->start(17); // ~60 FPS
}

void Window::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        vehicle->exiting = true;
        this->close();
    }
}

void Window::mousePressEvent(QMouseEvent *event) {

    // See if reset button was pressed
    if (sqrt(pow(415.0 - event->x(), 2) + pow(420.0 - event->y(), 2) * 1.0) < 30.0f) {
        qDebug() << "Trip reset pressed.";
        QMutexLocker locker(&vehicle->queueMutex);
        vehicle->serialWriteQueue.enqueue("trip");
        vehicle->setTripOdometer(0.0f);
    }

    // See if microcontroller button was pressed
    if (sqrt(pow(1240.0 - event->x(), 2) + pow(30.0 - event->y(), 2) * 1.0) < 60.0f) {
        qDebug() << "Microcontroller button pressed.";
        hwDialog->show();
    }
}

void Window::setHWDialog(HWDialog &hwDialog) {
    this->hwDialog = &hwDialog;
}
