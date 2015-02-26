#include "implicitline.h"

ImplicitLine::ImplicitLine(float (*fieldFunc)(float), float radius, float length) :
    ImplicitModel(fieldFunc),
    m_color(Qt::red),
    m_radius(radius),
    m_radiusSq(radius*radius),
    m_length(length),
    m_lengthSq(length*length),
    m_endPoint1(length/-2, 0.0f, 0.0f),
    m_endPoint2(length/2, 0.0f, 0.0f)
{
}

ImplicitLine::ImplicitLine(float (*fieldFunc)(float), QColor color, float radius, float length) :
    ImplicitModel(fieldFunc),
    m_color(color),
    m_radius(radius),
    m_radiusSq(radius*radius),
    m_length(length),
    m_lengthSq(length*length),
    m_endPoint1(length/-2, 0.0f, 0.0f),
    m_endPoint2(length/2, 0.0f, 0.0f)
{
}

ImplicitLine::ImplicitLine(float (*fieldFunc)(float), float fieldCoefficient, QColor color, float radius, float length) :
    ImplicitModel(fieldFunc, fieldCoefficient),
    m_color(color),
    m_radius(radius),
    m_radiusSq(radius*radius),
    m_length(length),
    m_lengthSq(length*length),
    m_endPoint1(length/-2, 0.0f, 0.0f),
    m_endPoint2(length/2, 0.0f, 0.0f)
{
}

float ImplicitLine::getFieldValue(QVector3D point)
{
    return m_fieldCoefficient * m_fieldFunc( (this->getDistanceSq(point))/(m_radiusSq));
}

float ImplicitLine::getDistanceSq(QVector3D point)
{
    /* If the line is really short look at it as a single point. */
    if(m_lengthSq <= 0.05f) {
        return ( (point.x()-m_endPoint1.x())*(point.x()-m_endPoint1.x()) ) +
                ( (point.y()-m_endPoint1.y())*(point.y()-m_endPoint1.y()) ) +
                ( (point.z()-m_endPoint1.z())*(point.z()-m_endPoint1.z()) );
    }
    /* Need the nearest point on the line to get a proper value */
    QVector3D nearestPoint = getNearestPoint(point);
    return ( (point.x()-nearestPoint.x())*(point.x()-nearestPoint.x()) ) +
            ( (point.y()-nearestPoint.y())*(point.y()-nearestPoint.y()) ) +
            ( (point.z()-nearestPoint.z())*(point.z()-nearestPoint.z()) );
}

float ImplicitLine::getDistance(QVector3D point)
{
    return qSqrt(getDistanceSq(point));
}

QVector3D ImplicitLine::getNearestPoint(QVector3D point) {
    /* Using a simple projection to find the nearest point on the line segment */
    float proj = QVector3D::dotProduct(point - m_endPoint1, m_endPoint2 - m_endPoint1) / m_lengthSq;
    if(proj < 0.0f) return m_endPoint1;
    if(proj > 1.0f) return m_endPoint2;
    return m_endPoint1 + proj*(m_endPoint2 - m_endPoint1);
}

bool ImplicitLine::containsPoint(QVector3D point, float errorMargin)
{
    return this->getDistanceSq(point) < (m_radius + errorMargin)*(m_radius + errorMargin);
}

QList<QVector3D> ImplicitLine::getPointsInObject()
{
    QList<QVector3D> result;
    result.append(QVector3D(0.0f, 0.0f, 0.0f));
    return result;
}

PointFlavour ImplicitLine::getFlavour(QVector3D point)
{
    QVector3D normal = point - getNearestPoint(point);
    return PointFlavour(m_color, normal.normalized());
}
