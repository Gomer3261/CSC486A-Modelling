#ifndef IMPLICITOBJECT_H
#define IMPLICITOBJECT_H

#include "qgl.h"
#include <QString>
#include <QMatrix4x4>
#include <QVector3D>
#include <QDebug>
#include <QWidget>
#include <QtMath>
#include <QColor>
#include <QList>

class PointFlavour
{
public:
    PointFlavour() :
        m_color(),
        m_normal()
    {}
    PointFlavour(QColor color, QVector3D normal) :
        m_color(color),
        m_normal(normal)
    {}

    QColor color() { return m_color; }
    QVector3D normal() { return m_normal; }
    PointFlavour& operator*=(const QMatrix4x4 rs) {
        m_normal = rs * m_normal;
        return *this;
    }
    PointFlavour& operator*=(const float rs) {
        m_normal = rs * m_normal;
        return *this;
    }
private:
    QColor m_color;
    QVector3D m_normal;
};

class ImplicitObject
{
public:
    virtual ~ImplicitObject() {}
    virtual float getFieldValue(QVector3D point) =0;
    virtual PointFlavour getFlavour(QVector3D point) =0;
    virtual bool containsPoint(QVector3D point, float errorMargin) =0;
    virtual QList<QVector3D> getPointsInObject() =0;
protected:
private:
};

#endif // IMPLICITOBJECT_H
