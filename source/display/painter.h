#ifndef PAINTER_H
#define PAINTER_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QCoreApplication>
#include <QImage>
#include <QMediaPlayer>
#include <QElapsedTimer>
#include "images.h"
#include "drawing.h"
#include "variables.h"
#include "vehiclevalues.h"

class Painter : public QObject
{
    Q_OBJECT

public:
    Painter(VehicleValues &vehicle, Indicators &indicators);

public:
    void paint(QPainter *painter, QPaintEvent *event);
    void initLoop();
    void updateIndicators();

private:
    QBrush backgroundBrush;
    QBrush shiftLightBrush;

    ImageMap *images;
    QFont largeGauge;
    QFont smallGauge;
    QFont tinyGauge;
    QFont normalText;
    QFont largeText;
    QPen gaugeTextPen;
    QPen lightTextPen;

    QElapsedTimer frameTimer;
    unsigned long long frameCount;
    Indicators *indicators;
    VehicleValues *vehicle;
    double brightness = 1.0;
};

#endif // PAINTER_H
