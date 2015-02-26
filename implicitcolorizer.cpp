#include "implicitcolorizer.h"

ImplicitColorizer::ImplicitColorizer(ImplicitObject *child) :
    ImplicitObject(),
    m_child(child)
{
}

float ImplicitColorizer::getFieldValue(QVector3D point) {
    return m_child->getFieldValue(point);
}

PointFlavour ImplicitColorizer::getFlavour(QVector3D point) {
    PointFlavour flavour = m_child->getFlavour(point);
    flavour = PointFlavour(transformColor(point, flavour.color()), flavour.normal());
    return flavour;
}

bool ImplicitColorizer::containsPoint(QVector3D point, float errorMargin) {
    return m_child->containsPoint(point, errorMargin);
}

QList<QVector3D> ImplicitColorizer::getPointsInObject() {
    return m_child->getPointsInObject();
}

void ImplicitColorizer::setChild(ImplicitObject* child) {
    m_child = child;
}
