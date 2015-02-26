#ifndef IMPLICITMODEL_H
#define IMPLICITMODEL_H

#include "implicitobject.h"

class ImplicitModel : public ImplicitObject
{
public:
    ImplicitModel(float (*fieldFunc)(float));
    ImplicitModel(float (*fieldFunc)(float), float coeff);

    void setFieldFunc(float (*fieldFunc)(float));
    void setFieldCofficient(float fieldCoefficient);

    virtual float getFieldValue(QVector3D point);
    virtual float getDistanceSq(QVector3D point) =0;
protected:
    float (*m_fieldFunc)(float);
    float m_fieldCoefficient;
};

#endif // IMPLICITMODEL_H
