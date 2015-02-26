#ifndef ORIBITALLIGHTSOURCE_H
#define ORIBITALLIGHTSOURCE_H

#include "qgl.h"
#include <QDebug>
#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>

class OrbitalLightSource
{
public:
    OrbitalLightSource(int lightId, QMatrix4x4 transform, float radius, float z, float rotationRate);
    void initializeGL();
    void paintGL();
protected:
private:
    int m_lightId;
    QVector3D m_center;
    QMatrix4x4 m_transform;
    float m_radius;
    float m_angle;
    float m_rotationRate;
};

#endif // ORIBITALLIGHTSOURCE_H
