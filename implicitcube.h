#ifndef IMPLICITCUBE_H
#define IMPLICITCUBE_H

#include "implicitmodel.h"

class ImplicitCube : public ImplicitModel
{
public:
public:
    ImplicitCube(float (*fieldFunc)(float), float radius, float sides);
    ImplicitCube(float (*fieldFunc)(float), QColor color, float radius, float sides);
    ImplicitCube(float (*fieldFunc)(float), float fieldCoefficient, QColor color, float radius, float sides);
    ImplicitCube(float (*fieldFunc)(float), float radius, float x, float y, float z);
    ImplicitCube(float (*fieldFunc)(float), QColor color, float radius, float x, float y, float z);
    ImplicitCube(float (*fieldFunc)(float), float fieldCoefficient, QColor color, float radius, float x, float y, float z);

    QVector3D getNearestPoint(QVector3D point);

    float getFieldValue(QVector3D point);
    float getDistanceSq(QVector3D point);
    float getDistance(QVector3D point);
    bool containsPoint(QVector3D point, float errorMargin);
    QList<QVector3D> getPointsInObject();
    PointFlavour getFlavour(QVector3D point);

protected:
    QColor m_color;
    float m_radius;
    float m_radiusSq;
    float m_x;
    float m_y;
    float m_z;
private:
};

#endif // IMPLICITCUBE_H
