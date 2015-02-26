#ifndef POLYGONIZERSCENE_H
#define POLYGONIZERSCENE_H

#include <QGLWidget>
#include <QMatrix4x4>
#include "orbitallightsource.h"
#include "model.h"
#include "polygonizer.h"

class PolygonizerScene
{
public:
    PolygonizerScene();
    ~PolygonizerScene();
    void initializeGL();
    void setModel(Model* model);
    void setVoxelSize(float size);
    void paintGL();
private:
    OrbitalLightSource *m_light;
    Polygonizer *m_polygonizer;
};

#endif // POLYGONIZERSCENE_H
