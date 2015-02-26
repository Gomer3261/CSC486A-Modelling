#include "implicitnoisewarp.h"

ImplicitNoiseWarp::ImplicitNoiseWarp(ImplicitObject *child, NoiseGenerator *generator, float coefficient) :
    ImplicitObject(),
    m_child(child),
    m_generator(generator),
    m_coefficient(coefficient)
{}

float ImplicitNoiseWarp::getFieldValue(QVector3D point)
{
    return m_child->getFieldValue(point) + (m_coefficient * m_generator->generateNoise(point.x(), point.y(), point.z()));
}

PointFlavour ImplicitNoiseWarp::getFlavour(QVector3D point)
{
    PointFlavour result = m_child->getFlavour(point);

    QVector3D normal = result.normal();
    QVector3D angle = QVector3D::crossProduct(result.normal(), QVector3D(0.1f, 0.0f, 0.0f));
    QMatrix4x4 trans;
    //Old brute force solution, ran over 200 times, new one runs 16 times.
    /*for(int i=-9; i<=9; i++) {
        trans.setToIdentity();
        trans.rotate(180 + i*10.0f, angle);
        for(int j=0; j<=12; j++) {
            trans.rotate(30.0f, result.normal());
            QVector3D sample = result.normal() * trans;
            float field = getFieldValue(point + 0.01f*sample);
            if(field > 0.5f) {
                normal += sample*-1*field;
            }
        }
    }*/

    qsrand((int)point.x() + (int)point.y()*57 + (int)point.z()*123);
    for(int i=0; i<4; i++) {
        trans.setToIdentity();
        trans.rotate(180 + i*15.0f + (qrand()%1500)/100.0f, angle);
        trans.rotate((qrand()%36000)/100.0f, result.normal());
        for(int j=0; j<4; j++) {
            trans.rotate(90.0f, result.normal());
            QVector3D sample = result.normal() * trans;
            float field = getFieldValue(point + 0.01f*sample);
            if(field > 0.5f) {
                normal += sample*-1*field;
            }
        }
    }

    return PointFlavour(result.color(), normal.normalized());
}

bool ImplicitNoiseWarp::containsPoint(QVector3D point, float errorMargin)
{
    return m_child->containsPoint(point, errorMargin);
}

QList<QVector3D> ImplicitNoiseWarp::getPointsInObject()
{
    return m_child->getPointsInObject();
}
