#include "implicitcube.h"

ImplicitCube::ImplicitCube(float (*fieldFunc)(float), float radius, float sides) :
    ImplicitModel(fieldFunc),
    m_color(Qt::red),
    m_radius(radius),
    m_radiusSq(radius*radius),
    m_x(sides/2.0f),
    m_y(sides/2.0f),
    m_z(sides/2.0f)
{
}

ImplicitCube::ImplicitCube(float (*fieldFunc)(float), QColor color, float radius, float sides) :
    ImplicitModel(fieldFunc),
    m_color(color),
    m_radius(radius),
    m_radiusSq(radius*radius),
    m_x(sides/2.0f),
    m_y(sides/2.0f),
    m_z(sides/2.0f)
{
}

ImplicitCube::ImplicitCube(float (*fieldFunc)(float), float fieldCoefficient, QColor color, float radius, float sides) :
    ImplicitModel(fieldFunc, fieldCoefficient),
    m_color(color),
    m_radius(radius),
    m_radiusSq(radius*radius),
    m_x(sides/2.0f),
    m_y(sides/2.0f),
    m_z(sides/2.0f)
{
}

ImplicitCube::ImplicitCube(float (*fieldFunc)(float), float radius, float x, float y, float z) :
    ImplicitModel(fieldFunc),
    m_color(Qt::red),
    m_radius(radius),
    m_radiusSq(radius*radius),
    m_x(x/2.0f),
    m_y(y/2.0f),
    m_z(z/2.0f)
{
}

ImplicitCube::ImplicitCube(float (*fieldFunc)(float), QColor color, float radius, float x, float y, float z) :
    ImplicitModel(fieldFunc),
    m_color(color),
    m_radius(radius),
    m_radiusSq(radius*radius),
    m_x(x/2.0f),
    m_y(y/2.0f),
    m_z(z/2.0f)
{
}

ImplicitCube::ImplicitCube(float (*fieldFunc)(float), float fieldCoefficient, QColor color, float radius, float x, float y, float z) :
    ImplicitModel(fieldFunc, fieldCoefficient),
    m_color(color),
    m_radius(radius),
    m_radiusSq(radius*radius),
    m_x(x/2.0f),
    m_y(y/2.0f),
    m_z(z/2.0f)
{
}

float ImplicitCube::getFieldValue(QVector3D point)
{
    return m_fieldCoefficient * m_fieldFunc( (this->getDistanceSq(point))/(m_radiusSq));
}

QVector3D ImplicitCube::getNearestPoint(QVector3D point)
{
    return QVector3D((point.x() < -m_x) ? -m_x : ( (point.x() > m_x) ? m_x : point.x() ),
                    (point.y() < -m_y) ? -m_y : ( (point.y() > m_y) ? m_y : point.y() ),
                    (point.z() < -m_z) ? -m_z : ( (point.z() > m_z) ? m_z : point.z() ));
}

float ImplicitCube::getDistanceSq(QVector3D point)
{
    QVector3D nearestPoint = getNearestPoint(point);
    return ( (point.x()-nearestPoint.x())*(point.x()-nearestPoint.x()) ) +
            ( (point.y()-nearestPoint.y())*(point.y()-nearestPoint.y()) ) +
            ( (point.z()-nearestPoint.z())*(point.z()-nearestPoint.z()) );
}

float ImplicitCube::getDistance(QVector3D point)
{
    return qSqrt(getDistanceSq(point));
}

bool ImplicitCube::containsPoint(QVector3D point, float errorMargin)
{
    return this->getDistanceSq(point) < ((m_radius + errorMargin)*(m_radius + errorMargin));
}

QList<QVector3D> ImplicitCube::getPointsInObject()
{
    QList<QVector3D> result;
    result.append(QVector3D(0.0f, 0.0f, 0.0f));
    return result;
}

PointFlavour ImplicitCube::getFlavour(QVector3D point)
{
    QVector3D normal = point - getNearestPoint(point);
    if(normal.length() < 0.01f) {
        if(qAbs(point.x()) > qAbs(point.y()) && qAbs(point.x()) > qAbs(point.z())) {
            normal = QVector3D(point.x(), 0.0f, 0.0f);
        } else if (qAbs(point.y()) > qAbs(point.z())) {
            normal = QVector3D(0.0f, point.y(), 0.0f);
        } else {
            normal = QVector3D(0.0f, 0.0f, point.z());
        }
    }
    return PointFlavour(m_color, normal.normalized());
}


