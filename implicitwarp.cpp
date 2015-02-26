#include "implicitwarp.h"
#include "qmath.h"

ImplicitWarp::ImplicitWarp(ImplicitObject *child, QVector3D offset, QVector3D direction) :
    ImplicitObject(),
    m_child(child),
    m_transform()
{
    m_transform.setToIdentity();

    QVector3D up(0.0f, 0.0f, 1.0f);
    if(direction != up) {
        m_transform.lookAt(offset, offset + direction, up);
    }

    bool invertable;
    m_inverse = m_transform.inverted(&invertable);
}

QVector3D ImplicitWarp::warpPoint(QVector3D point)
{
    point = point * m_transform;
    return warpFunction(point) * m_inverse;
}

float ImplicitWarp::getFieldValue(QVector3D point)
{
    return m_child->getFieldValue(warpPoint(point));
}

PointFlavour ImplicitWarp::getFlavour(QVector3D point)
{
    PointFlavour result = m_child->getFlavour(warpPoint(point));
    QMatrix4x4 trans;
    QVector3D normal(0.0f, 0.0f, 0.0f);

    qsrand((int)point.x() + (int)point.y()*57 + (int)point.z()*123);
    for(int i=0; i<9; i++) {
        trans.setToIdentity();
        trans.rotate(i*20.0f + (qrand()%2000)/100.0f, QVector3D(0.0f, 0.0f, 1.0f));
        trans.rotate((qrand()%36000)/100.0f, QVector3D(0.0f, 1.0f, 0.0f));
        for(int j=0; j<8; j++) {
            float rot = (qrand()%4500)/100.0f;
            trans.rotate(rot, QVector3D(0.0f, 1.0f, 0.0f));
            QVector3D sample = QVector3D(0.0f, 1.0f, 0.0f) * trans;
            float field = m_child->getFieldValue(warpPoint(point + 0.05f*sample));
            normal += sample*-1.0f*field;
            trans.rotate(45.0f-rot, QVector3D(0.0f, 1.0f, 0.0f));
        }
    }

    return PointFlavour(result.color(), normal.normalized());
}

bool ImplicitWarp::containsPoint(QVector3D point, float errorMargin) {
    point = warpPoint(point);
    return m_child->containsPoint(point, errorMargin);
}

QList<QVector3D> ImplicitWarp::getPointsInObject() {
    return m_child->getPointsInObject();
}
