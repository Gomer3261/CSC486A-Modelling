#include "implicittransform.h"

ImplicitTransform::ImplicitTransform(ImplicitObject* child) :
    ImplicitObject(),
    m_child(child),
    m_transform(),
    m_inverse(),
    m_normalT()
{
}

ImplicitTransform::ImplicitTransform(ImplicitObject* child, QMatrix4x4 transform) :
    ImplicitObject(),
    m_child(child),
    m_transform(transform),
    m_inverse(),
    m_normalT()
{
    setTransform(transform);
}

void ImplicitTransform::setTransform(QMatrix4x4 transform) {
    m_transform = transform;

    ImplicitTransform *trans = dynamic_cast<ImplicitTransform*>(m_child);
    if(trans != NULL) {
        //qDebug() << "Child transform!";
        m_transform = transform * trans->getTransform();
        m_child = trans->getChild();
    } else {
        //qDebug() << "Not child transform!";
        m_transform = transform;
    }

    bool success = false;
    m_inverse = m_transform.inverted(&success);
    m_normalT = m_inverse.transposed();
    m_normalT.setColumn(3, QVector4D(0.0f, 0.0f, 0.0f, m_normalT.column(3).w()));
    m_normalT.setRow(3, QVector4D(0.0f, 0.0f, 0.0f, m_normalT.row(3).w()));
}

QMatrix4x4 ImplicitTransform::getTransform() {
    return m_transform;
}

ImplicitObject* ImplicitTransform::getChild() {
    return m_child;
}

float ImplicitTransform::getFieldValue(QVector3D point) {
    applyInverseTransform(&point);
    return m_child->getFieldValue(point);
}

PointFlavour ImplicitTransform::getFlavour(QVector3D point) {
    applyInverseTransform(&point);
    return (m_child->getFlavour(point) *= m_normalT);
}

bool ImplicitTransform::containsPoint(QVector3D point, float errorMargin) {
    applyInverseTransform(&point);
    return m_child->containsPoint(point, errorMargin);
}

QList<QVector3D> ImplicitTransform::getPointsInObject() {
    QList<QVector3D> points = m_child->getPointsInObject();
    for(int i=0; i<points.size(); i++) {
        applyTransform(&(points[i]));
    }
    return points;
}

void ImplicitTransform::applyTransform(QVector3D *point) {
    *point = m_transform*(*point);
}

void ImplicitTransform::applyInverseTransform(QVector3D *point) {
    *point = m_inverse*(*point);
}
