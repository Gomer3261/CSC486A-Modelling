#ifndef IMPLICITTRANSFORM_H
#define IMPLICITTRANSFORM_H

#include "implicitobject.h"
#include <QMatrix4x4>

class ImplicitTransform : public ImplicitObject
{
public:
    ImplicitTransform(ImplicitObject* child, QMatrix4x4 transform);

    float getFieldValue(QVector3D point);
    PointFlavour getFlavour(QVector3D point);
    bool containsPoint(QVector3D point, float errorMargin);
    QList<QVector3D> getPointsInObject();


protected:
    ImplicitTransform(ImplicitObject* child);
    void setTransform(QMatrix4x4 transform);

    QMatrix4x4 getTransform();
    ImplicitObject* getChild();

    void applyTransform(QVector3D *point);
    void applyInverseTransform(QVector3D *point);    

    ImplicitObject* m_child;
    QMatrix4x4 m_transform;
    QMatrix4x4 m_inverse;
    QMatrix4x4 m_normalT;
private:
};

class ImplicitTranslate : public ImplicitTransform
{
public:
    ImplicitTranslate(ImplicitObject *child, QVector3D translate) :
        ImplicitTransform(child)
    {
        QMatrix4x4 transform;
        transform.translate(translate);
        setTransform(transform);
    }

    ImplicitTranslate(ImplicitObject* child, float x, float y, float z) :
        ImplicitTransform(child)
    {
        QMatrix4x4 transform;
        transform.translate(QVector3D(x, y, z));
        setTransform(transform);
    }
};

class ImplicitRotate : public ImplicitTransform
{
public:
    ImplicitRotate(ImplicitObject *child, float x, float y, float z) :
        ImplicitTransform(child)
    {
        QMatrix4x4 transform;
        transform.rotate(x, 1.0f, 0.0f, 0.0f);
        transform.rotate(y, 0.0f, 1.0f, 0.0f);
        transform.rotate(z, 0.0f, 0.0f, 1.0f);
        setTransform(transform);
    }

    ImplicitRotate(ImplicitObject *child, float degree, QVector3D vector) :
        ImplicitTransform(child)
    {
        QMatrix4x4 transform;
        transform.rotate(degree, vector);
        setTransform(transform);
    }
};

class ImplicitScale : public ImplicitTransform
{
public:
    ImplicitScale(ImplicitObject *child, float scale) :
        ImplicitTransform(child)
    {
        QMatrix4x4 transform;
        transform.scale(scale);
        setTransform(transform);
    }

    ImplicitScale(ImplicitObject *child, float x, float y, float z) :
        ImplicitTransform(child)
    {
        QMatrix4x4 transform;
        transform.scale(x, y, z);
        setTransform(transform);
    }

    ImplicitScale(ImplicitObject *child,  QVector3D scale) :
        ImplicitTransform(child)
    {
        QMatrix4x4 transform;
        transform.scale(scale);
        setTransform(transform);
    }
};

#endif // IMPLICITTRANSFORM_H
