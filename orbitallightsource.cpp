#include "orbitallightsource.h"

OrbitalLightSource::OrbitalLightSource(int lightId, QMatrix4x4 transform, float radius, float z, float rotationRate) :
    m_lightId(lightId),
    m_center(0, 0, z),
    m_transform(transform),
    m_radius(radius),
    m_angle(0),
    m_rotationRate(rotationRate)
{
}

void OrbitalLightSource::initializeGL()
{
    /* Initialize openGL light values for provided light id. */
    GLfloat lightPos[] = {m_center.x(), m_center.y(), m_center.z(), 1.0f};
    GLfloat lightSpec[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightDif[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat lightAmb[] = {0.25f, 0.25f, 0.25f, 1.0f};

    glLightfv(m_lightId, GL_POSITION, lightPos);
    glLightfv(m_lightId, GL_SPECULAR, lightSpec);
    glLightfv(m_lightId, GL_DIFFUSE, lightDif);
    glLightfv(m_lightId, GL_AMBIENT, lightAmb);
}

void OrbitalLightSource::paintGL()
{
    /* Actually used to animate light location before the rest of drawing occurs. */
    m_angle += m_rotationRate;
    if(m_angle > M_PI*2) {
        m_angle -= M_PI*2;
    }

    QVector3D position = m_transform * QVector3D(m_center.x() + m_radius*qCos(m_angle), m_center.y() + m_radius*qSin(m_angle), m_center.z());
    GLfloat lightPos[] = {position.x(), position.y(), position.z(), 1.0f};
    glLightfv(m_lightId, GL_POSITION, lightPos);
}
