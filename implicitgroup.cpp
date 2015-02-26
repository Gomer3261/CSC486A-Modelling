#include "implicitgroup.h"

ImplicitGroup::ImplicitGroup() :
    ImplicitObject(),
    m_recursiveObjects(),
    m_baseObjects(),
    m_maxDepth(0),
    m_depth(0)
{
}

ImplicitGroup::ImplicitGroup(int maxDepth) :
    ImplicitObject(),
    m_recursiveObjects(),
    m_baseObjects(),
    m_maxDepth(maxDepth),
    m_depth(0)
{
}

void ImplicitGroup::addBaseObject(ImplicitObject* object) {
    m_baseObjects.append(object);
}

void ImplicitGroup::addRecursiveObject(ImplicitObject* object) {
    m_recursiveObjects.append(object);
}

float ImplicitGroup::getFieldValue(QVector3D point) {
    float fieldSum = 0.0f;
    if(m_depth < m_maxDepth) {
        m_depth++;
        fieldSum = getFieldFromObjects(m_recursiveObjects, point);
        m_depth--;
    } else if(m_depth == m_maxDepth) {
        m_depth++;
        fieldSum = getFieldFromObjects(m_baseObjects, point);
        m_depth--;
    }
    return fieldSum;
}

PointFlavour ImplicitGroup::getFlavour(QVector3D point) {
    PointFlavour flavour;
    if(m_depth < m_maxDepth) {
        m_depth++;
        flavour = getFlavourFromObjects(m_recursiveObjects, point);
        m_depth--;
    } else if(m_depth == m_maxDepth) {
        m_depth++;
        flavour = getFlavourFromObjects(m_baseObjects, point);
        m_depth--;
    }
    return flavour;
}

bool ImplicitGroup::containsPoint(QVector3D point, float errorMargin) {
    bool result = false;
    if(m_depth < m_maxDepth) {
        m_depth++;
        result = containedInObjects(m_recursiveObjects, point, errorMargin);
        m_depth--;
    } else if(m_depth == m_maxDepth) {
        m_depth++;
        result = containedInObjects(m_baseObjects, point, errorMargin);
        m_depth--;
    }
    return result;
}

QList<QVector3D> ImplicitGroup::getPointsInObject()
{
    QList<QVector3D> result;
    if(m_depth < m_maxDepth) {
        m_depth++;
        result = getPointsInObjects(m_recursiveObjects);
        m_depth--;
    } else if(m_depth == m_maxDepth) {
        m_depth++;
        result = getPointsInObjects(m_baseObjects);
        m_depth--;
    }
    return result;
}

bool ImplicitGroup::containedInObjects(QList<ImplicitObject*> objects, QVector3D point, float errorMargin) {
    for(int i=0; i<objects.size(); i++) {
        if(objects[i]->containsPoint(point, errorMargin)) {
            return true;
        }
    }
    return false;
}

QList<QVector3D> ImplicitGroup::getPointsInObjects(QList<ImplicitObject*> objects) {
    QList<QVector3D> points;
    for(int i=0; i<objects.size(); i++) {
        points.append(objects[i]->getPointsInObject());
    }
    return points;
}

PointFlavour ImplicitGroup::getFlavourFromObjects(QList<ImplicitObject*> objects, QVector3D point) {
    QColor color(0.0f, 0.0f, 0.0f);
    QVector3D normal(0.0f, 0.0f, 0.0f);

    float fieldSum = 0.0f;
    float field[objects.size()];

    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;

    for(int i=0; i<objects.size(); i++) {
        field[i] = objects[i]->getFieldValue(point);
        fieldSum += field[i];
        PointFlavour sample = objects[i]->getFlavour(point);
        normal += sample.normal()*field[i];
        r += sample.color().redF()*field[i];
        g += sample.color().greenF()*field[i];
        b += sample.color().blueF()*field[i];
    }

    if(fieldSum == 0.0f) fieldSum = 1.0f;
    normal.normalize();
    color.setRedF(r/fieldSum);
    color.setGreenF(g/fieldSum);
    color.setBlueF(b/fieldSum);

    return PointFlavour(color, normal);
}

/* ---------------------------------------- *
 * ----------------- BLEND ---------------- *
 * ---------------------------------------- */
float ImplicitBlend::getFieldFromObjects(QList<ImplicitObject*> objects, QVector3D point) {
    float fieldSum = 0.0f;
    for(int i=0; i<objects.size(); i++) {
        fieldSum += objects[i]->getFieldValue(point);
    }
    return fieldSum;
}

/* ---------------------------------------- *
 * ----------------- RICCI ---------------- *
 * ---------------------------------------- */
float ImplicitBlendRicci::getFieldFromObjects(QList<ImplicitObject*> objects, QVector3D point) { //TODO: Implement RICCI blend
    double fieldSum = 0.0f;
    for(int i=0; i<objects.size(); i++) {
        fieldSum += qPow(objects[i]->getFieldValue(point), m_power);
    }
    if(fieldSum == 0.0f) return fieldSum;
    return qPow(fieldSum, m_invPower);
}

/* ---------------------------------------- *
 * ----------------- UNION ---------------- *
 * ---------------------------------------- */
/*TODO:
 * - Add a bit of color blending between objects to reduce alias effect.
 */
float ImplicitUnion::getFieldFromObjects(QList<ImplicitObject*> objects, QVector3D point) {
    float field;
    float result = 0.0f;
    for(int i=0; i<objects.size(); i++) {
        field = objects[i]->getFieldValue(point);
        result = (field > result) ? field : result;
    }
    return result;
}

PointFlavour ImplicitUnion::getFlavourFromObjects(QList<ImplicitObject*> objects, QVector3D point) {
    PointFlavour result;
    float field;
    float maxField = 0.0f;
    for(int i=0; i<objects.size(); i++) {
        field = objects[i]->getFieldValue(point);
        if(field > maxField) {
            maxField = field;
            result = objects[i]->getFlavour(point);
        }
    }
    return result;
}

/* ---------------------------------------- *
 * --------------- INTERSECT -------------- *
 * ---------------------------------------- */
/* TODO:
 * - Figure out if it can look a little prettier, maybe work on color blending.
 */
float ImplicitIntersect::getFieldFromObjects(QList<ImplicitObject*> objects, QVector3D point) {
    float field;
    float result = 1000.0f;
    for(int i=0; i<objects.size(); i++) {
        field = objects[i]->getFieldValue(point);
        result = (field < result) ? field : result;
    }
    return result;
}

PointFlavour ImplicitIntersect::getFlavourFromObjects(QList<ImplicitObject*> objects, QVector3D point) {
    PointFlavour result;
    float field;
    float minField = 1000.0f;
    for(int i=0; i<objects.size(); i++) {
        field = objects[i]->getFieldValue(point);
        if(field < minField) {
            minField = field;
            result = objects[i]->getFlavour(point);
        }
    }
    return result;
}

QList<QVector3D> ImplicitIntersect::getPointsInObjects(QList<ImplicitObject*> objects) {
    QList<QVector3D> points;
    QVector3D point;
    int sum = 0;
    for(int i=0; i<objects.size(); i++) {
        QList<QVector3D> objPoints = objects[i]->getPointsInObject();
        for(int j=0; j<objPoints.size(); j++) {
            sum++;
            point += objPoints[j];
        }
    }
    points.append(point / sum);
    return points;
}

/* ---------------------------------------- *
 * -------------- DIFFERENCE -------------- *
 * ---------------------------------------- */
/*TODO:
 * - Points in object generally fails if center is cut by implicit.
 * - Make search function check all 6 directions before giving up. (New function, return when found)
 */

float ImplicitDifference::getFieldFromObjects(QList<ImplicitObject*> objects, QVector3D point) {
    if(objects.size() == 0) return 0.0f;
    float field = objects[0]->getFieldValue(point);
    float result = field;
    for(int i=1; i<objects.size(); i++) {
        field = (2 * m_iso) - objects[i]->getFieldValue(point);
        result = (field < result) ? field : result;
    }
    return result;
}

PointFlavour ImplicitDifference::getFlavourFromObjects(QList<ImplicitObject*> objects, QVector3D point) {
    if(objects.size() == 0) return PointFlavour();
    float field = objects[0]->getFieldValue(point);
    float fieldMin = field;
    PointFlavour result = objects[0]->getFlavour(point);
    for(int i=1; i<objects.size(); i++) {
        field = (2 * m_iso) - (objects[i]->getFieldValue(point) + 0.01f);
        if(field <= fieldMin) {
            fieldMin = field;
            result = objects[i]->getFlavour(point);
            result *= -1;
        }
    }
    return result;
}

bool ImplicitDifference::containedInObjects(QList<ImplicitObject*> objects, QVector3D point, float errorMargin) {
    if(objects.size() == 0) return false;
    if(!objects[0]->containsPoint(point, errorMargin)) return false;
    return true;
}

QList<QVector3D> ImplicitDifference::getPointsInObjects(QList<ImplicitObject*> objects) {
    if(objects.size() == 0) return QList<QVector3D>();
    return objects[0]->getPointsInObject();
}
