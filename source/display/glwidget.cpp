#include "glwidget.h"

GLWidget::GLWidget(Painter *painter, QWidget *parent) : QOpenGLWidget(parent), painter(painter)
{
    elapsed = 0;
    setFixedSize(1280, 480);
    setAutoFillBackground(false);
}

void GLWidget::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    update();
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    QPainter localPainter;
    localPainter.begin(this);
    localPainter.setRenderHint(QPainter::Antialiasing);
    painter->paint(&localPainter, event);
    localPainter.end();
}
