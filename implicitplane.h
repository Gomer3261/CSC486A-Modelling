#ifndef IMPLICITPLANE_H
#define IMPLICITPLANE_H

#include "implicitmodel.h"

class ImplicitPlane : public ImplicitModel
{
public:
    ImplicitPlane(float (*fieldFunc)(float), float radius);
    ImplicitPlane(float (*fieldFunc)(float), QColor color, float radius);
    ImplicitPlane(float (*fieldFunc)(float), float fieldCoefficient, QColor color, float radius);

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
private:
};

#endif // IMPLICITPLANE_H
