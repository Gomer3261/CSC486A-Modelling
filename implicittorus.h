#ifndef IMPLICITTORUS_H
#define IMPLICITTORUS_H

#include "implicitmodel.h"

class ImplicitTorus : public ImplicitModel
{
public:
    ImplicitTorus(float (*fieldFunc)(float), float innerRadius, float outerRadius);
    ImplicitTorus(float (*fieldFunc)(float), QColor color, float innerRadius, float outerRadius);
    ImplicitTorus(float (*fieldFunc)(float), float fieldCoefficient, QColor color, float innerRadius, float outerRadius);

    QVector3D getNearestPoint(QVector3D point);

    float getFieldValue(QVector3D point);
    float getDistanceSq(QVector3D point);
    float getDistance(QVector3D point);
    bool containsPoint(QVector3D point, float errorMargin);
    QList<QVector3D> getPointsInObject();
    PointFlavour getFlavour(QVector3D point);

protected:
    QColor m_color;
    float m_innerRadius;
    float m_outerRadius;
    float m_width;
    float m_widthSq;
    float m_centerRadius;
private:
};

#endif // IMPLICITTORUS_H
