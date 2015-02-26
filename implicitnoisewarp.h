#ifndef IMPLICITNOISEWARP_H
#define IMPLICITNOISEWARP_H

#include "implicitobject.h"
#include "perlinnoise.h"

class ImplicitNoiseWarp : public ImplicitObject
{
public:
    ImplicitNoiseWarp(ImplicitObject *child, NoiseGenerator *generator, float coefficient);

    float getFieldValue(QVector3D point);
    PointFlavour getFlavour(QVector3D point);
    bool containsPoint(QVector3D point, float errorMargin);
    QList<QVector3D> getPointsInObject();
protected:
private:
    ImplicitObject *m_child;
    NoiseGenerator *m_generator;
    float m_coefficient;
};

#endif // IMPLICITNOISEWARP_H
