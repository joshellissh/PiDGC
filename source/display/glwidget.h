#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include "painter.h"

class Painter;

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget(Painter *painter, QWidget *parent);

public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Painter *painter;
    int elapsed;
};

#endif // GLWIDGET_H
