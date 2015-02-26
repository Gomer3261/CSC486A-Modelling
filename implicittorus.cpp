#include "implicittorus.h"

ImplicitTorus::ImplicitTorus(float (*fieldFunc)(float), float innerRadius, float outerRadius) :
    ImplicitModel(fieldFunc),
    m_color(Qt::red),
    m_innerRadius(innerRadius),
    m_outerRadius(outerRadius)
{
    m_width = outerRadius - innerRadius;
    m_widthSq = m_width * m_width;
    m_centerRadius = innerRadius + m_width/2;
}

ImplicitTorus::ImplicitTorus(float (*fieldFunc)(float), QColor color, float innerRadius, float outerRadius) :
    ImplicitModel(fieldFunc),
    m_color(color),
    m_innerRadius(innerRadius),
    m_outerRadius(outerRadius)
{
    m_width = outerRadius - innerRadius;
    m_widthSq = m_width * m_width;
    m_centerRadius = innerRadius + m_width/2;
}

ImplicitTorus::ImplicitTorus(float (*fieldFunc)(float), float fieldCoefficient, QColor color, float innerRadius, float outerRadius) :
    ImplicitModel(fieldFunc, fieldCoefficient),
    m_color(color),
    m_innerRadius(innerRadius),
    m_outerRadius(outerRadius)
{
    m_width = outerRadius - innerRadius;
    m_widthSq = m_width * m_width;
    m_centerRadius = innerRadius + m_width/2;
}

QVector3D ImplicitTorus::getNearestPoint(QVector3D point) {
    /* Using a simple projection to find the nearest point on the line segment */
    QVector3D result = point;
    result.setZ(0.0f);
    result.normalize();
    return result * m_centerRadius;
}

float ImplicitTorus::getFieldValue(QVector3D point)
{
    return m_fieldCoefficient * m_fieldFunc( (this->getDistanceSq(point))/(m_widthSq*0.25f));
}

float ImplicitTorus::getDistanceSq(QVector3D point)
{
    QVector3D nearestPoint = getNearestPoint(point);
    return ( (point.x()-nearestPoint.x())*(point.x()-nearestPoint.x()) ) +
            ( (point.y()-nearestPoint.y())*(point.y()-nearestPoint.y()) ) +
            ( (point.z()-nearestPoint.z())*(point.z()-nearestPoint.z()) );
}

float ImplicitTorus::getDistance(QVector3D point)
{
    return qSqrt(getDistanceSq(point));
}

bool ImplicitTorus::containsPoint(QVector3D point, float errorMargin)
{
    return getDistanceSq(point) <= ((m_width/2)+errorMargin)*((m_width/2)+errorMargin);
}

QList<QVector3D> ImplicitTorus::getPointsInObject()
{
    QList<QVector3D> result;
    result.append(QVector3D(m_centerRadius, 0.0f, 0.0f));
    return result;
}

PointFlavour ImplicitTorus::getFlavour(QVector3D point)
{
    QVector3D normal = point - getNearestPoint(point);
    return PointFlavour(m_color, normal.normalized());
}
