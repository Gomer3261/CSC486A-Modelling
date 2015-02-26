#ifndef IMPLICITLINE_H
#define IMPLICITLINE_H

#include "implicitmodel.h"

class ImplicitLine : public ImplicitModel
{
public:
    ImplicitLine(float (*fieldFunc)(float), float radius, float length);
    ImplicitLine(float (*fieldFunc)(float), QColor color, float radius, float length);
    ImplicitLine(float (*fieldFunc)(float), float fieldCoefficient, QColor color, float radius, float length);

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
    float m_length;
    float m_lengthSq;
    QVector3D m_endPoint1;
    QVector3D m_endPoint2;
private:
};

#endif // IMPLICITLINE_H
