#ifndef IMPLICITWARP_H
#define IMPLICITWARP_H

#include "implicitobject.h"
#include <QVector4D>
#include <qmath.h>

class ImplicitWarp : public ImplicitObject
{
public:
    ImplicitWarp(ImplicitObject *child, QVector3D offset, QVector3D direction);
    float getFieldValue(QVector3D point);
    PointFlavour getFlavour(QVector3D point);
    bool containsPoint(QVector3D point, float errorMargin);
    QList<QVector3D> getPointsInObject();

    QVector3D warpPoint(QVector3D point);
    virtual QVector3D warpFunction(QVector3D point)=0;
protected:
    ImplicitObject *m_child;
    QMatrix4x4 m_transform;
    QMatrix4x4 m_inverse;
private:
};

class ImplicitTwist : public ImplicitWarp
{
public:
    ImplicitTwist(ImplicitObject *child) : ImplicitWarp(child, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)), m_coefficient(1.0f) {}
    ImplicitTwist(ImplicitObject *child, float coefficient) : ImplicitWarp(child, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)), m_coefficient(coefficient) {}
    ImplicitTwist(ImplicitObject *child, QVector3D offset, QVector3D direction, float coefficient)  : ImplicitWarp(child, offset, direction), m_coefficient(coefficient) {}
    QVector3D warpFunction(QVector3D point) {
        return QVector3D(
                    ( point.x()*qCos(m_coefficient*point.z()) ) - ( point.y()*qSin(m_coefficient*point.z()) ),
                    ( point.x()*qSin(m_coefficient*point.z()) ) + ( point.y()*qCos(m_coefficient*point.z()) ),
                    point.z());
    }
private:
    float m_coefficient;
};

class ImplicitTaper : public ImplicitWarp
{
public:
    ImplicitTaper(ImplicitObject *child) : ImplicitWarp(child, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)), m_min(1.0f), m_max(0.0f) {}
    ImplicitTaper(ImplicitObject *child, float max, float min) : ImplicitWarp(child, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)), m_min(max), m_max(min) {}
    ImplicitTaper(ImplicitObject *child, QVector3D offset, QVector3D direction, float max, float min)  : ImplicitWarp(child, offset, direction), m_min(max), m_max(min) {}
    QVector3D warpFunction(QVector3D point) {
        float scale = (m_max - point.z()) / (m_max - m_min);
        return QVector3D(
                    ( point.x()/scale ),
                    ( point.y()/scale ),
                    point.z());
    }
private:
    float m_min;
    float m_max;
};

class ImplicitBend : public ImplicitWarp
{
public:
    ImplicitBend(ImplicitObject *child) : ImplicitWarp(child, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)), m_rate(1.0f) {}
    ImplicitBend(ImplicitObject *child, float rate) : ImplicitWarp(child, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)), m_rate(rate) {}
    ImplicitBend(ImplicitObject *child, QVector3D offset, QVector3D direction, float rate)  : ImplicitWarp(child, offset, direction), m_rate(rate) {}
    QVector3D warpFunction(QVector3D point) {
        return QVector3D(
                    ( -0.1f * qSin( (point.x() + 1.5f)*m_rate )*(point.y() - 1.0f/m_rate) - 1.5f ),
                    ( qCos( (point.x() + 1.5f)*m_rate )*(point.y() - 1.0f/m_rate) + 1.0f/m_rate ),
                    point.z()); //Pretty sure this is just wrong math...
    }
private:
    float m_rate;
};

#endif // IMPLICITWARP_H
