#include "implicitsphere.h"

ImplicitSphere::ImplicitSphere(float (*fieldFunc)(float), float radius) :
    ImplicitModel(fieldFunc),
    m_color(Qt::red),
    m_radius(radius),
    m_radiusSq(radius*radius)
{
}

ImplicitSphere::ImplicitSphere(float (*fieldFunc)(float), QColor color, float radius) :
    ImplicitModel(fieldFunc),
    m_color(color),
    m_radius(radius),
    m_radiusSq(radius*radius)
{
}

ImplicitSphere::ImplicitSphere(float (*fieldFunc)(float), float fieldCoefficient, QColor color, float radius) :
    ImplicitModel(fieldFunc, fieldCoefficient),
    m_color(color),
    m_radius(radius),
    m_radiusSq(radius*radius)
{
}

float ImplicitSphere::getFieldValue(QVector3D point)
{
    /* field functions expect normalized distance values. */
    return m_fieldCoefficient * m_fieldFunc( (this->getDistanceSq(point))/(m_radiusSq));
}

float ImplicitSphere::getDistance(QVector3D point)
{
    return point.distanceToPoint(QVector3D());
}

float ImplicitSphere::getDistanceSq(QVector3D point)
{
    /* distanceSq is faster as no sqrt is necessary. */
    return ( point.x()*point.x() ) + ( point.y()*point.y() ) + ( point.z()*point.z() );
}

bool ImplicitSphere::containsPoint(QVector3D point, float errorMargin)
{
    return this->getDistanceSq(point) < (m_radius+errorMargin)*(m_radius+errorMargin);
}

QList<QVector3D> ImplicitSphere::getPointsInObject()
{
    QList<QVector3D> result;
    result.append(QVector3D(0.0f, 0.0f, 0.0f));
    return result;
}

PointFlavour ImplicitSphere::getFlavour(QVector3D point) {
    return PointFlavour(m_color, point.normalized());
}
