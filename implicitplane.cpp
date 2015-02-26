#include "implicitplane.h"

ImplicitPlane::ImplicitPlane(float (*fieldFunc)(float), float radius) :
    ImplicitModel(fieldFunc),
    m_color(Qt::red),
    m_radius(radius)
{
}

ImplicitPlane::ImplicitPlane(float (*fieldFunc)(float), QColor color, float radius) :
    ImplicitModel(fieldFunc),
    m_color(color),
    m_radius(radius)
{
}
ImplicitPlane::ImplicitPlane(float (*fieldFunc)(float), float fieldCoefficient, QColor color, float radius) :
    ImplicitModel(fieldFunc, fieldCoefficient),
    m_color(color),
    m_radius(radius)
{
}

float ImplicitPlane::getFieldValue(QVector3D point)
{
    return m_fieldCoefficient * m_fieldFunc( (this->getDistanceSq(point))/(m_radiusSq));
}
float ImplicitPlane::getDistanceSq(QVector3D point)
{
    return getDistance(point) * getDistance(point);
}

float ImplicitPlane::getDistance(QVector3D point)
{
    return (point.y() < 0.0f) ? point.y()/m_radius : 0.0f;
}

bool ImplicitPlane::containsPoint(QVector3D point, float errorMargin)
{
    return point.y() < m_radius+errorMargin;
}

QList<QVector3D> ImplicitPlane::getPointsInObject()
{
    return QList<QVector3D>();
}

PointFlavour ImplicitPlane::getFlavour(QVector3D point)
{
    (void)point;
    return PointFlavour(m_color, QVector3D(0.0f, 1.0f, 0.0f));
}
