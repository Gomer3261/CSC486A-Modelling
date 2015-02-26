#ifndef IMPLICITSPHERE_H
#define IMPLICITSPHERE_H

#include "implicitmodel.h"

class ImplicitSphere : public ImplicitModel
{
public:
    ImplicitSphere(float (*fieldFunc)(float), float radius);
    ImplicitSphere(float (*fieldFunc)(float), QColor color, float radius);
    ImplicitSphere(float (*fieldFunc)(float), float fieldCoefficient, QColor color, float radius);

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

#endif // IMPLICITSPHERE_H
