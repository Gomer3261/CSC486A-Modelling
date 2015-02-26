#ifndef IMPLICITGROUP_H
#define IMPLICITGROUP_H

#include "implicitobject.h"
#include <QList>

class ImplicitGroup : public ImplicitObject
{
public:
    ImplicitGroup();
    ImplicitGroup(int maxDepth);

    void addBaseObject(ImplicitObject* object);
    void addRecursiveObject(ImplicitObject* object);

    float getFieldValue(QVector3D point);
    PointFlavour getFlavour(QVector3D point);
    bool containsPoint(QVector3D point, float errorMargin);
    QList<QVector3D> getPointsInObject();

    virtual float getFieldFromObjects(QList<ImplicitObject*> objects, QVector3D point)=0;
    virtual PointFlavour getFlavourFromObjects(QList<ImplicitObject*> objects, QVector3D point);
    virtual bool containedInObjects(QList<ImplicitObject*> objects, QVector3D point, float errorMargin);
    virtual QList<QVector3D> getPointsInObjects(QList<ImplicitObject*> objects);
private:
    QList<ImplicitObject*> m_recursiveObjects;
    QList<ImplicitObject*> m_baseObjects;
    int m_maxDepth;
    int m_depth;
private:
};

class ImplicitBlend : public ImplicitGroup
{
public:
    ImplicitBlend() : ImplicitGroup() {}
    ImplicitBlend(int maxDepth) : ImplicitGroup(maxDepth) {}

    float getFieldFromObjects(QList<ImplicitObject*> objects, QVector3D point);
};

class ImplicitBlendRicci : public ImplicitGroup
{
public:
    ImplicitBlendRicci(float power) :
        ImplicitGroup(),
        m_power(power),
        m_invPower(1.0f/power)
    {}
    ImplicitBlendRicci(float power, int maxDepth) :
        ImplicitGroup(maxDepth),
        m_power(power),
        m_invPower(1.0f/power)
    {}

    float getFieldFromObjects(QList<ImplicitObject*> objects, QVector3D point);
private:
    float m_power;
    float m_invPower;
};

class ImplicitUnion : public ImplicitGroup
{
public:
    ImplicitUnion() : ImplicitGroup() {}
    ImplicitUnion(int maxDepth) : ImplicitGroup(maxDepth) {}

    float getFieldFromObjects(QList<ImplicitObject*> objects, QVector3D point);
    PointFlavour getFlavourFromObjects(QList<ImplicitObject*> objects, QVector3D point);
};

class ImplicitIntersect : public ImplicitGroup
{
public:
    ImplicitIntersect() : ImplicitGroup() {}
    ImplicitIntersect(int maxDepth) : ImplicitGroup(maxDepth) {}

    float getFieldFromObjects(QList<ImplicitObject*> objects, QVector3D point);
    PointFlavour getFlavourFromObjects(QList<ImplicitObject*> objects, QVector3D point);
    QList<QVector3D> getPointsInObjects(QList<ImplicitObject*> objects);
};

class ImplicitDifference : public ImplicitGroup
{
public:
    ImplicitDifference(float iso) :
        ImplicitGroup(),
        m_iso(iso)
    {}
    ImplicitDifference(float iso, int maxDepth) :
        ImplicitGroup(maxDepth),
        m_iso(iso)
    {}

    float getFieldFromObjects(QList<ImplicitObject*> objects, QVector3D point);
    PointFlavour getFlavourFromObjects(QList<ImplicitObject*> objects, QVector3D point);
    bool containedInObjects(QList<ImplicitObject*> objects, QVector3D point, float errorMargin);
    QList<QVector3D> getPointsInObjects(QList<ImplicitObject*> objects);
private:
    float m_iso;
};

#endif // IMPLICITGROUP_H
