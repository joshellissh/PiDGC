#ifndef DRAWING_H
#define DRAWING_H

#include <QPainter>
#include <math.h>

#define PI  3.1415926f

inline float degToRad(float degrees) {
     return degrees * (PI / 180.0f);
}

inline void drawFromCenter(QPainter *painter, QImage image) {
    painter->drawImage(QRect(-image.width()/2, -image.height()/2, image.width(), image.height()), image);
}

inline void drawCenteredAt(QPainter *painter, QImage image, float x, float y) {
    painter->save();
    painter->translate(x, y);
    drawFromCenter(painter, image);
    painter->restore();
}

inline void drawCenteredWithOffsetAndRotation(QPainter *painter, QImage image, float x, float y, float offsetX, float offsetY, float rotation) {
    painter->save();
    painter->translate(x, y);
    painter->rotate(rotation);
    painter->translate(offsetX, offsetY);
    drawFromCenter(painter, image);
    painter->restore();
}

inline void drawTorus(QPainter *painter, qreal xCenter, qreal yCenter, float innerRadius, float outerRadius, float startArcRad, float endArcRad, int segments) {
    painter->save();
    painter->translate(xCenter, yCenter);
    painter->rotate(90.0f);

    QPoint innerPoints[2048];
    int pointIndex = 0;
    for(int ii = 0; ii < segments; ii++) {
        float theta = 2.0f * PI * float(ii) / float(segments);//get the current angle

        if (theta > endArcRad || theta < startArcRad) continue;

        float x = innerRadius * cos(theta);//calculate the x component
        float y = innerRadius * sin(theta);//calculate the y component

        innerPoints[pointIndex++] = QPoint(x, y);//output vertex
    }

    QPoint outerPoints[2048];
    pointIndex = 0;
    for(int ii = segments; ii >= 0; ii--) {
        float theta = 2.0f * PI * float(ii) / float(segments);//get the current angle

        if (theta > endArcRad || theta < startArcRad) continue;

        float x = outerRadius * cos(theta);//calculate the x component
        float y = outerRadius * sin(theta);//calculate the y component

        outerPoints[pointIndex++] = QPoint(x, y);//output vertex
    }

    QPoint allPoints[2048];
    int allIndex = 0;
    for(int i = 0; i < pointIndex; i++) {
        allPoints[allIndex++] = innerPoints[i];
    }
    for(int i = 0; i < pointIndex; i++) {
        allPoints[allIndex++] = outerPoints[i];
    }

    painter->drawPolygon(allPoints, allIndex);

    painter->restore();
}

#endif // DRAWING_H
