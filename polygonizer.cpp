#include "polygonizer.h"

Polygonizer::Polygonizer(ImplicitObject* object) :
    m_object(object),
    m_voxels(),
    m_drawableVoxels(),
    m_vertexData(),
    m_vboId(0),
    m_vertexCount(0),
    m_dirty(false),
    m_voxelSize(0.05f),
    m_fieldThreshold(0.5f),
    m_debug(true)
{
}

Polygonizer::Polygonizer(ImplicitObject* object, float voxelSize) :
    m_object(object),
    m_voxels(),
    m_drawableVoxels(),
    m_vertexData(),
    m_vboId(0),
    m_vertexCount(0),
    m_dirty(false),
    m_voxelSize(voxelSize),
    m_fieldThreshold(0.5f),
    m_debug(true)
{
}

Polygonizer::~Polygonizer()
{
    glDeleteBuffersARB(1, &m_vboId);
}

void Polygonizer::setObject(ImplicitObject* object)
{
    m_object = object;
    setDirtyFlag();
}

void Polygonizer::setDirtyFlag()
{
    /* Used to inform the polygonizer that it must be run before rendering. */
    glDeleteBuffersARB(1, &m_vboId);
    m_dirty = true;
}

void Polygonizer::setDebug(bool debug)
{
    m_debug = debug;
}

void Polygonizer::setVoxelSize(float size)
{
    qDebug() << "Voxel size set!" << size;
    m_voxelSize = size;
    setDirtyFlag();
}

void Polygonizer::setFieldThreshold(float threshold)
{
    m_fieldThreshold = threshold;
    setDirtyFlag();
}

void Polygonizer::voxelize()
{
    /* Stack used to store voxels that need testing. */
    QStack<PolygonizerVoxel*> voxelStack = QStack<PolygonizerVoxel*>();

    /* Reset Voxels */
    QHashIterator<uint, PolygonizerVoxel*> i(m_voxels);
    while (i.hasNext()) {
        i.next();
        delete(i.value());
    }
    m_voxels.clear();
    m_drawableVoxels.clear();
    for(int i=0; i<m_vertexData.size(); i++) {
        delete m_vertexData[i];
    }
    m_vertexData.clear();
    m_vertexCount = 0;

    /* Add a starting point from each object in the scene. */

    qDebug() << "\nLooking for starter voxels!";
    //qDebug() << "Points in object" << m_object->getPointsInObject().size();
    for(int startIndex=0; startIndex < m_object->getPointsInObject().size(); startIndex++) {
        QVector3D startPoint = m_object->getPointsInObject()[startIndex];
        //qDebug() << startPoint;
        PolygonizerVoxel *starterVoxel = new PolygonizerVoxel(
                    (int)(startPoint.x()/m_voxelSize),
                    (int)(startPoint.y()/m_voxelSize),
                    (int)(startPoint.z()/m_voxelSize));
        if(!isInObjects(starterVoxel)) {
            //qDebug() << "Point not initially in object!";
            delete starterVoxel;
            continue;
        }

        findFirstShellVoxel(starterVoxel);

        if(starterVoxel->garbage()) {
            //qDebug() << "Point is garbage!";
            delete starterVoxel;
            continue;
        }
        /* No reason to add the same voxel twice on overlapping objects. */
        if(!m_voxels.contains(qHash(*starterVoxel), starterVoxel)) {
            m_voxels.insert(qHash(*starterVoxel), starterVoxel);
            voxelStack.push(starterVoxel);
        } else {
            //qDebug() << "Voxel already exists!";
            delete starterVoxel;
        }
    }
    qDebug() << voxelStack.size() << "starter voxels found!";

    int tested = 0;

    /* Find all voxels that contain part of the model */
    while(!voxelStack.isEmpty()) {
        PolygonizerVoxel testVoxel;
        PolygonizerVoxel *workingVoxel;

        PolygonizerVoxel *voxel = voxelStack.pop();
        if(!voxel->visited()) {
            /* Test each corner of the voxel to see if it's in the model. */
            int corners = 0;

            for(int i=0; i<8; i++) {
                if(i == BACK_BOTTOM_LEFT) {
                    workingVoxel = voxel;
                } else {
                    testVoxel.mutateVoxel(voxel->x() + m_cornerOffsets[i].o_x,
                                          voxel->y() + m_cornerOffsets[i].o_y,
                                          voxel->z() + m_cornerOffsets[i].o_z);
                    workingVoxel = findOrCreateVoxel(testVoxel);
                }
                if(!workingVoxel->tested()) {
                    workingVoxel->setField(BACK_BOTTOM_LEFT, getFieldValue(workingVoxel));
                    workingVoxel->setTested();
                }
                voxel->setField(i, workingVoxel->field(BACK_BOTTOM_LEFT));
                /*qDebug() << "---  Testing corner" << i << "field:" << workingVoxel->field()
                            << "voxel:" << workingVoxel->x() << workingVoxel->y() << workingVoxel->z();*/
                if(workingVoxel->field(BACK_BOTTOM_LEFT) > m_fieldThreshold) corners = corners | (1 << i);
            }

            /*qDebug() << "Testing voxel: " << voxel->x() << " : " << voxel->y() << " : " << voxel->z() << " corners:" << corners;*/
            tested++;

            /* Mark this voxel as processed. */
            voxel->setCorners(corners);
            voxel->setVisited();

            /* If the voxel contains part of the model, we should draw it! :D */
            if(corners > 0) {
                m_drawableVoxels.append(voxel);
                m_vertexCount += m_polygonMap[corners].count*3;
            }

            /* Add adjacent voxels that will contain part of the model. */
            for(int i=0; i<6; i++) {
                if( ((m_faceOffsetMap[i].face & corners) > 0) && ((m_faceOffsetMap[i].face & corners) != m_faceOffsetMap[i].face) ) {
                    testVoxel.mutateVoxel(voxel->x() + m_faceOffsetMap[i].o_x,
                                          voxel->y() + m_faceOffsetMap[i].o_y,
                                          voxel->z() + m_faceOffsetMap[i].o_z);
                    workingVoxel = findOrCreateVoxel(testVoxel);
                    if(!workingVoxel->visited()) {
                        voxelStack.append(workingVoxel);
                    }
                }
            }

            /* Create vertex data storage for edges that need to be drawn. */
            for(int i=0; i<12; i++) {
                if( ((m_edgeMap[i].mask & corners) > 0) && ((m_edgeMap[i].mask & corners) != m_edgeMap[i].mask) ) {
                    testVoxel.mutateVoxel(voxel->x() + m_faceOffsetMap[m_edgeAdjacentMap[i].f1].o_x,
                                          voxel->y() + m_faceOffsetMap[m_edgeAdjacentMap[i].f1].o_y,
                                          voxel->z() + m_faceOffsetMap[m_edgeAdjacentMap[i].f1].o_z);
                    workingVoxel = findOrCreateVoxel(testVoxel);
                    if(workingVoxel->vertex(m_edgeAdjacentMap[i].e1) != NULL) {
                        voxel->setVertex(i, workingVoxel->vertex(m_edgeAdjacentMap[i].e1));
                        continue;
                    }

                    testVoxel.mutateVoxel(voxel->x() + m_faceOffsetMap[m_edgeAdjacentMap[i].f2].o_x,
                                          voxel->y() + m_faceOffsetMap[m_edgeAdjacentMap[i].f2].o_y,
                                          voxel->z() + m_faceOffsetMap[m_edgeAdjacentMap[i].f2].o_z);
                    workingVoxel = findOrCreateVoxel(testVoxel);
                    if(workingVoxel->vertex(m_edgeAdjacentMap[i].e2) != NULL) {
                        voxel->setVertex(i, workingVoxel->vertex(m_edgeAdjacentMap[i].e2));
                        continue;
                    }
                    VertexData* newData = new VertexData();
                    voxel->setVertex(i, newData);
                    m_vertexData.append(newData);
                }
            }
        }
    }

    qDebug() << "\n" << tested << "drawable voxels tested!";
    qDebug() << "" << m_drawableVoxels.count() << "drawable voxels found!\n";
}

void Polygonizer::findFirstShellVoxel(PolygonizerVoxel* starterVoxel) {
    bool inside = (getFieldValue(starterVoxel) >= m_fieldThreshold);

    PolygonizerVoxel *testVoxel = new PolygonizerVoxel(starterVoxel->x(), starterVoxel->y(), starterVoxel->z());

    int depth;
    for(int i=0; i<6; i++) {
        depth = 0;
        bool pos = (m_faceOffsetMap[i].o_x + m_faceOffsetMap[i].o_y + m_faceOffsetMap[i].o_z) > 0; //Positive or negative, axis agnositc.
        while(isInObjects(testVoxel)) {
            depth++;
            if((!inside && (getFieldValue(testVoxel) >= m_fieldThreshold)) ||
                    (inside && (getFieldValue(testVoxel) < m_fieldThreshold))) {
                if(pos) {
                    starterVoxel->mutateVoxel(testVoxel->x()+(m_faceOffsetMap[i].o_x*-1),
                                              testVoxel->y()+(m_faceOffsetMap[i].o_y*-1),
                                              testVoxel->z()+(m_faceOffsetMap[i].o_z*-1));
                } else {
                    starterVoxel->mutateVoxel(testVoxel->x(), testVoxel->y(), testVoxel->z());
                }
                return;
            }
            testVoxel->mutateVoxelOffset(m_faceOffsetMap[i].o_x, m_faceOffsetMap[i].o_y, m_faceOffsetMap[i].o_z);
            if(depth >= 100) {
                //qDebug() << "Depth limit hit!";
                break;
            }
        }
        testVoxel->mutateVoxel(starterVoxel->x(), starterVoxel->y(), starterVoxel->z());
        //qDebug() << "Depth Reached: " << depth << "Direction:" << m_faceOffsetMap[i].o_x << m_faceOffsetMap[i].o_y << m_faceOffsetMap[i].o_z;
    }

    starterVoxel->setGarbage();
}

PolygonizerVoxel* Polygonizer::findVoxel(const PolygonizerVoxel &testVoxel)
{
    QMultiHash<uint, PolygonizerVoxel*>::iterator iter = m_voxels.find(qHash(testVoxel));
    while(iter != m_voxels.end() && iter.key() == qHash(testVoxel)) {
        if((*iter.value()) == testVoxel) {
            return iter.value();
        }
        ++iter;
    }
    return NULL;
}

PolygonizerVoxel* Polygonizer::findOrCreateVoxel(const PolygonizerVoxel &testVoxel)
{
    /* Checking for an existing voxel */
    PolygonizerVoxel* voxel = findVoxel(testVoxel);
    if(voxel != NULL) return voxel;

    /* Creating a voxel if necessary and placing in the table */
    voxel = new PolygonizerVoxel(testVoxel.x(), testVoxel.y(), testVoxel.z());
    m_voxels.insert(qHash(*voxel), voxel);
    return voxel;
}

float Polygonizer::getFieldValue(PolygonizerVoxel *voxel)
{
    return m_object->getFieldValue(QVector3D(voxel->x()*m_voxelSize, voxel->y()*m_voxelSize, voxel->z()*m_voxelSize));
}

bool Polygonizer::isInObjects(PolygonizerVoxel *voxel)
{
    return m_object->containsPoint(QVector3D(voxel->x()*m_voxelSize, voxel->y()*m_voxelSize, voxel->z()*m_voxelSize), 3*m_voxelSize);
}

void Polygonizer::polygonize() {
    int index = 0;

    float *vertices = new float[3*m_vertexCount];
    float *normals = new float[3*m_vertexCount];
    float *colours = new float[3*m_vertexCount];

    for(int i=0; i<m_drawableVoxels.size(); i++) {

        /* Required vertices for each polygon/model situation are stored in a table */
        for(int j=0; j<m_polygonMap[m_drawableVoxels[i]->corners()].count*3; j++) {
            int vertex = m_polygonMap[m_drawableVoxels[i]->corners()].polygons[j];
            VertexData *data = m_drawableVoxels[i]->vertex(vertex);
            if(data != NULL) {
                if(data->initialized) {
                    vertices[3*index] = data->position.x(); vertices[(3*index) + 1] = data->position.y(); vertices[(3*index) + 2] = data->position.z();
                    normals[3*index] = data->normal.x(); normals[(3*index) + 1] = data->normal.y(); normals[(3*index) + 2] = data->normal.z();
                    colours[3*index] = data->colour.redF(); colours[(3*index) + 1] = data->colour.greenF(); colours[(3*index) + 2] = data->colour.blueF();
                    index++;
                    continue;
                }
            }
            QVector3D v1( (m_drawableVoxels[i]->x() + m_cornerOffsets[m_edgeMap[vertex].v1].o_x)*m_voxelSize,
                    (m_drawableVoxels[i]->y() + m_cornerOffsets[m_edgeMap[vertex].v1].o_y)*m_voxelSize,
                    (m_drawableVoxels[i]->z() + m_cornerOffsets[m_edgeMap[vertex].v1].o_z)*m_voxelSize);
            QVector3D v2( (m_drawableVoxels[i]->x() + m_cornerOffsets[m_edgeMap[vertex].v2].o_x)*m_voxelSize,
                    (m_drawableVoxels[i]->y() + m_cornerOffsets[m_edgeMap[vertex].v2].o_y)*m_voxelSize,
                    (m_drawableVoxels[i]->z() + m_cornerOffsets[m_edgeMap[vertex].v2].o_z)*m_voxelSize);

            QVector3D result = interpolateVertex(v1, v2, m_drawableVoxels[i]->field(m_edgeMap[vertex].v1), m_drawableVoxels[i]->field(m_edgeMap[vertex].v2));

            PointFlavour flavour = m_object->getFlavour(result);
            QVector3D normal = flavour.normal().normalized();
            vertices[3*index] = result.x(); vertices[(3*index) + 1] = result.y(); vertices[(3*index) + 2] = result.z();
            normals[3*index] = normal.x(); normals[(3*index) + 1] = normal.y(); normals[(3*index) + 2] = normal.z();
            colours[3*index] = flavour.color().redF(); colours[(3*index) + 1] = flavour.color().greenF(); colours[(3*index) + 2] = flavour.color().blueF();

            index++;

            if(data != NULL) {
                data->position = result;
                data->normal = flavour.normal();
                data->colour = flavour.color();
                data->initialized = true;
            }
        }
    }

    generateVBO(vertices, normals, colours);

    delete vertices;
    delete normals;
    delete colours;
}

QVector3D Polygonizer::interpolateVertex(QVector3D v1, QVector3D v2, float field1, float field2)
{
    float v = (0.5f - field1) / (field2 - field1);
    return v1 + (v2 - v1)*v;
}

void Polygonizer::generateVBO(float *vertices, float *normals, float *colours)
{
    if(m_vboId == 0) {
        qDebug() << "Creating VBO pointer.";
        glGenBuffersARB(1, &m_vboId);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vboId);
    } else {
        qDebug() << "Freeing VBO pointer.";
        glDeleteBuffersARB(1, &m_vboId);
        glGenBuffersARB(1, &m_vboId);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vboId);
    }
    qDebug() << "Generating VBO";
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, 9*m_vertexCount*sizeof(float), 0, GL_DYNAMIC_DRAW); //GL_STATIC_DRAW_ARB
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, 3*m_vertexCount*sizeof(float), vertices);
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 3*m_vertexCount*sizeof(float), 3*m_vertexCount*sizeof(float), normals);
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 6*m_vertexCount*sizeof(float), 3*m_vertexCount*sizeof(float), colours);
}

void Polygonizer::paintGL()
{
    /* Polygonize if necessary. */
    if(m_dirty) {
        QElapsedTimer timer;
        timer.start();
        voxelize();
        qDebug() << "Voxelization time: " << timer.nsecsElapsed()/1000000.0f << "ms";
        timer.restart();
        polygonize();
        qDebug() << "Polygonization time: " << timer.nsecsElapsed()/1000000.0f << "ms";
        m_dirty = false;
    }

    /* Initialize materials. */
    GLfloat spec[] = {0.0f, 0.0f, 0.0f, 0.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);

    QElapsedTimer timer;
    timer.start();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    /* Begin Drawing Voxels */
    glEnable(GL_COLOR_MATERIAL);
    drawVBO();

    /* Disable lighting */
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    //qDebug() << "Draw time: " << timer.nsecsElapsed()/1000000.0f << "ms";

    if(m_debug) {
        /* Draw voxels for debugging. */
        float cornerOffset = m_voxelSize/10.0f;
        float innerEdge = m_voxelSize*8.0f/10.0f;

        glColor3f(0.1f, 0.1f, 0.1f);
        glPointSize(5.0f);
        glBegin(GL_POINTS);
        for(int i=0; i<m_drawableVoxels.size(); i++) {
            for(int j=0; j<8; j++) {
                /* Draw points for each corner found to be inside the object. */
                if( (m_drawableVoxels[i]->corners() & (1 << j) ) >= 0) {
                    glVertex3f(m_drawableVoxels[i]->x()*m_voxelSize + cornerOffset + m_cornerOffsets[j].o_x*innerEdge,
                               m_drawableVoxels[i]->y()*m_voxelSize + cornerOffset + m_cornerOffsets[j].o_y*innerEdge,
                               m_drawableVoxels[i]->z()*m_voxelSize + cornerOffset + m_cornerOffsets[j].o_z*innerEdge);
                }
            }
        }
        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f);
    }
}

void Polygonizer::drawVBO()
{
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vboId);

    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glNormalPointer(GL_FLOAT, 0, (void*)(3*m_vertexCount*sizeof(float)) );
    glColorPointer(3, GL_FLOAT, 0, (void*)(6*m_vertexCount*sizeof(float)) );
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}
