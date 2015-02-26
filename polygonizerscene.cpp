#include "polygonizerscene.h"

PolygonizerScene::PolygonizerScene() :
    m_light(NULL),
    m_polygonizer(NULL)
{
    QMatrix4x4 lightTransform;
    lightTransform.rotate(-90, 1, 0, 0);
    m_light = new OrbitalLightSource(GL_LIGHT0, lightTransform, 10.0f, 5.0f, 0.01f);

    m_polygonizer = new Polygonizer(NULL, 0.01f);

    m_polygonizer->setDirtyFlag();
    m_polygonizer->setDebug(false);
}

PolygonizerScene::~PolygonizerScene()
{
    delete m_light;
    delete m_polygonizer;
}

void PolygonizerScene::setModel(Model* model)
{
    m_polygonizer->setObject(model->getRoot());
}

void PolygonizerScene::setVoxelSize(float size)
{
    m_polygonizer->setVoxelSize(size);
}

void PolygonizerScene::initializeGL()
{
    m_light->initializeGL();
}

void PolygonizerScene::paintGL()
{
    m_light->paintGL();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    m_polygonizer->paintGL();
}
