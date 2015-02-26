#include "implicitmodel.h"

ImplicitModel::ImplicitModel(float (*fieldFunc)(float)) :
    ImplicitObject(),
    m_fieldFunc(fieldFunc),
    m_fieldCoefficient(1.0f)
{
}

ImplicitModel::ImplicitModel(float (*fieldFunc)(float), float fieldCoefficient) :
    ImplicitObject(),
    m_fieldFunc(fieldFunc),
    m_fieldCoefficient(fieldCoefficient)
{
}

float ImplicitModel::getFieldValue(QVector3D point)
{
    return m_fieldCoefficient * m_fieldFunc(this->getDistanceSq(point));
}

void ImplicitModel::setFieldFunc(float (*fieldFunc)(float))
{
    m_fieldFunc = fieldFunc;
}

void ImplicitModel::setFieldCofficient(float fieldCoefficient)
{
    m_fieldCoefficient = fieldCoefficient;
}
