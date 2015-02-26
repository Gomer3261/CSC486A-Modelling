#ifndef POLYGONIZER_H
#define POLYGONIZER_H

#include "qgl.h"
#include <QVector3D>
#include <QDebug>
#include <QList>
#include <QStack>
#include <QMultiHash>
#include <QtMath>
#include "polygonizervoxel.h"
#include "implicitobject.h"
#include "QElapsedTimer"

class Polygonizer
{
public:
    Polygonizer(ImplicitObject* object);
    Polygonizer(ImplicitObject* object, float voxelSize);
    ~Polygonizer();
    void setObject(ImplicitObject* object);
    void setDirtyFlag();
    void setDebug(bool debug);
    void paintGL();
    void setVoxelSize(float size);
    void setFieldThreshold(float threshold);
protected:
private:
    void initialize();
    void voxelize();
    void polygonize();
    void findFirstShellVoxel(PolygonizerVoxel* starterVoxel);
    PolygonizerVoxel* findVoxel(const PolygonizerVoxel &testVoxel);
    PolygonizerVoxel* findOrCreateVoxel(const PolygonizerVoxel &testVoxel);
    float getFieldValue(PolygonizerVoxel* voxel);
    bool isInObjects(PolygonizerVoxel *voxel);
    void generateVBO(float *vertices, float *normals, float*colours);
    void drawVBO();
    QVector3D interpolateVertex(QVector3D v1, QVector3D v2, float field1, float field2);

    ImplicitObject* m_object;
    QMultiHash<uint, PolygonizerVoxel*> m_voxels;
    QList<PolygonizerVoxel*> m_drawableVoxels;
    QList<VertexData*> m_vertexData;
    GLuint m_vboId;
    int m_vertexCount;
    bool m_dirty;
    float m_voxelSize;
    float m_fieldThreshold;
    bool m_debug;

    struct corneroffset_t {
        int o_x;
        int o_y;
        int o_z;
    };
    const static corneroffset_t m_cornerOffsets[];

    struct facemap_t {
        int face;
        int o_x;
        int o_y;
        int o_z;
    };
    const static facemap_t m_faceOffsetMap[];

    struct edgeadjmap_t {
        int f1;
        int e1;
        int f2;
        int e2;
    };
    const static edgeadjmap_t m_edgeAdjacentMap[];

    struct edgemap_t {
        int v1;
        int v2;
        int mask;
    };
    const static edgemap_t m_edgeMap[];

    struct polygonmap_t {
        int count;
        int polygons[15];
    };
    const static polygonmap_t m_polygonMap[];
};

#endif // POLYGONIZER_H
