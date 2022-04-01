#ifndef WINDOW_H
#define WINDOW_H

#include "painter.h"
#include <QWidget>
#include "glwidget.h"
#include <QLabel>
#include <QGridLayout>
#include <QKeyEvent>
#include "hwdialog.h"

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(VehicleValues &vehicle);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void setHWDialog(HWDialog &hwDialog);

private:
    Painter *painter;
    VehicleValues *vehicle;
    HWDialog *hwDialog;
};

#endif // WINDOW_H
