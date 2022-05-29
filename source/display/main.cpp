#include "window.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QThread>
#include <QProcess>
#include "hwdialog.h"
#include "serialcontroller.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QProcess::execute("pulseaudio -D");

    // Turn on anti-aliasing
    QSurfaceFormat fmt;
    fmt.setSamples(4);
    QSurfaceFormat::setDefaultFormat(fmt);

    VehicleValues vehicle;

    // Create window
    Window window(vehicle);
    window.setCursor(Qt::BlankCursor);
    window.show();

    // Create hardware dialog
    HWDialog hwDialog(vehicle, &window);

    // Store dialog reference in various windows
    window.setHWDialog(hwDialog);

    // Start serial interface
    SerialController serialController(vehicle);
    serialController.start();

    return app.exec();
}
