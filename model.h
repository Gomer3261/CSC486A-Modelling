#ifndef MODEL_H
#define MODEL_H

#include <QList>
#include "perlinnoise.h"
#include "fieldfunctions.h"
#include "implicitobject.h"
#include "implicitplane.h"
#include "implicitsphere.h"
#include "implicitline.h"
#include "implicitcube.h"
#include "implicittorus.h"
#include "implicittransform.h"
#include "implicitgroup.h"
#include "implicitcolorizer.h"
#include "implicitnoisewarp.h"
#include "implicitwarp.h"

class Model
{
public:
    Model() : m_objects(), m_generators(), m_root(NULL) {}
    ~Model() { for(int i=0; i<m_objects.size(); i++) { delete m_objects[i]; }
             for(int i=0; i<m_generators.size(); i++) { delete m_generators[i]; } }
    ImplicitObject* getRoot() { return m_root; }

    /* Do not call unless you are constructing a new model instance. */
    ImplicitObject* setRoot(ImplicitObject* root) { m_root = root; return root; }
    ImplicitObject* addObject(ImplicitObject* object) { m_objects.append(object); return object; }
    ImplicitGroup* addGroup(ImplicitGroup* group) { m_objects.append(group); return group; }
    NoiseGenerator* addGenerator(NoiseGenerator* generator) { m_generators.append(generator); return generator; }
private:
    QList<ImplicitObject*> m_objects;
    QList<NoiseGenerator*> m_generators;
    ImplicitObject* m_root;
};

class SnowmanModel : public Model
{
public:
    SnowmanModel(int depth) : Model() {
        NoiseGenerator *snowGenerator = addGenerator(new PerlinNoise(.9f));
        ImplicitGroup *body = addGroup(new ImplicitBlendRicci(4.0f, depth));
        ImplicitObject *bodySphere = addObject(new ImplicitSphere(fieldfunctions::geoffFunction, 1.0f));
        ImplicitObject *texture = addObject(new ImplicitNoiseColorizer(bodySphere, snowGenerator,
                                                                        QColor::fromRgbF(0.90f, 0.90f, 0.90f),
                                                                        QColor::fromRgbF(0.7f, 0.7f, 1.0f))
                                            );
        ImplicitObject *buttonSphere = addObject(new ImplicitSphere(fieldfunctions::wyvillFunction, Qt::red, 0.1f));
        ImplicitObject *button1 = addObject(new ImplicitTranslate(buttonSphere, 0.0f, .15f, -.49f));
        ImplicitObject *button2 = addObject(new ImplicitTranslate(buttonSphere, 0.0f, -.15f, -.49f));
        ImplicitObject *scale = addObject(new ImplicitScale(body, 0.8f));
        ImplicitObject *translate = addObject(new ImplicitTranslate(scale, 0.0f, 0.6f, 0.0f));
        body->addRecursiveObject(texture);
        body->addRecursiveObject(translate);
        body->addRecursiveObject(button1);
        body->addRecursiveObject(button2);

        ImplicitGroup *head = addGroup(new ImplicitUnion());
        ImplicitObject *eyeSphere = addObject(new ImplicitSphere(fieldfunctions::wyvillFunction, Qt::black, 0.16f));
        ImplicitObject *eyeL = addObject(new ImplicitTranslate(eyeSphere, -.20f, .12f, -.41f));
        ImplicitObject *eyeR = addObject(new ImplicitTranslate(eyeSphere, .20f, .12f, -.41f));
        head->addBaseObject(texture);
        head->addBaseObject(eyeL);
        head->addBaseObject(eyeR);

        body->addBaseObject(head);

        NoiseGenerator *woodGenerator = addGenerator(new GrainNoise());
        ImplicitObject *stick = addObject(new ImplicitLine(fieldfunctions::wyvillFunction, 0.1f, 1.0f));
        ImplicitGroup *armBase = addGroup(new ImplicitBlendRicci(10.0f, 1));
        ImplicitObject *armColor = addObject(new ImplicitNoiseShadeColorizer(armBase, woodGenerator, QColor(139, 69, 19)));
        ImplicitObject *armRot1 = addObject(new ImplicitRotate(armColor, 0.0f, 0.0f, 151.0f));
        ImplicitObject *armRot2 = addObject(new ImplicitRotate(armColor, 0.0f, 0.0f, 43.0f));
        ImplicitObject *arm1 = addObject(new ImplicitTranslate(armRot1, -0.85f, -0.50, 0));
        ImplicitObject *arm2 = addObject(new ImplicitTranslate(armRot2, 0.8f, -0.50, 0));
        ImplicitObject *hand = addObject(new ImplicitTranslate(armBase, 0.5f, 0.0f, 0.0f));
        ImplicitObject *finger = addObject(new ImplicitScale(stick, 0.3f, 0.7f, 0.7f));
        ImplicitObject *fingerTranslate = addObject(new ImplicitTranslate(finger, 0.15f, 0.0f, 0.0f));
        ImplicitObject *finger1 = addObject(new ImplicitRotate(fingerTranslate, 0.0f, 30.0f, 0.0f));
        ImplicitObject *finger2 = addObject(new ImplicitRotate(fingerTranslate, 120.0f, 30.0f, 0.0f));
        ImplicitObject *finger3 = addObject(new ImplicitRotate(fingerTranslate, 240.0f, 30.0f, 0.0f));
        armBase->addBaseObject(finger1);
        armBase->addBaseObject(finger2);
        armBase->addBaseObject(finger3);
        armBase->addRecursiveObject(stick);
        armBase->addRecursiveObject(hand);

        body->addBaseObject(arm1);
        body->addBaseObject(arm2);
        setRoot(body);
    }
};

class RainbowModel : public Model
{
public:
    RainbowModel() : Model() {
        ImplicitObject *torus = addObject(new ImplicitTorus(fieldfunctions::blinnFunction, Qt::red, 1.4f, 2.0f));
        ImplicitGroup *rainbow = addGroup(new ImplicitBlend(6));
        ImplicitObject *shift = addObject(new ImplicitHueShiftColorizer(rainbow, 0.16f));
        ImplicitObject *scale = addObject(new ImplicitScale(shift, 0.85f, 0.85f, 1.0f));
        rainbow->addRecursiveObject(torus);
        rainbow->addRecursiveObject(scale);

        ImplicitGroup *half = addGroup(new ImplicitDifference(0));
        ImplicitObject *plane = addObject(new ImplicitPlane(fieldfunctions::blinnFunction, Qt::black, 0.5f));
        ImplicitObject *planeOffset = addObject(new ImplicitTranslate(plane, 0.0f, 0.0f, -0.4f));
        half->addBaseObject(rainbow);
        half->addBaseObject(planeOffset);
        ImplicitObject *root = addObject(new ImplicitTranslate(half, 0.0f, -0.5f, 0.0f));
        setRoot(root);
    }
};

class CutCubeModel : public Model {
public:
    CutCubeModel(int depth) : Model() {
        ImplicitObject *cube = addObject(new ImplicitCube(fieldfunctions::blinnFunction, Qt::blue, 0.5f, 2.0f));
        ImplicitObject *cutCube = addObject(new ImplicitCube(fieldfunctions::blinnFunction, Qt::yellow, 0.1f, 0.7f));
        ImplicitGroup *cutGroup = addGroup(new ImplicitUnion(depth));
        ImplicitObject *offsetCut = addObject(new ImplicitTranslate(cutGroup, 2.25f, 2.25f, -0.85f));
        ImplicitObject *scaleCut = addObject(new ImplicitScale(offsetCut, 0.33f));
        ImplicitObject *topLeft = addObject(new ImplicitRotate(scaleCut, 0.0f, 0.0f, 270.0f));
        ImplicitObject *bottomLeft = addObject(new ImplicitRotate(scaleCut, 0.0f, 0.0f, 180.0f));
        ImplicitObject *bottomRight = addObject(new ImplicitRotate(scaleCut, 0.0f, 0.0f, 90.0f));
        cutGroup->addRecursiveObject(cutCube);
        cutGroup->addRecursiveObject(topLeft);
        cutGroup->addRecursiveObject(scaleCut);
        cutGroup->addRecursiveObject(bottomLeft);
        cutGroup->addRecursiveObject(bottomRight);

        ImplicitGroup *root = addGroup(new ImplicitDifference(0.5f, 0));
        ImplicitObject *front = addObject(new ImplicitTranslate(cutGroup, 0.0f, 0.0f, -0.85f));
        ImplicitObject *top = addObject(new ImplicitRotate(front, 90.0f, 0.0f, 0.0f));
        ImplicitObject *back = addObject(new ImplicitRotate(front, 180.0f, 0.0f, 0.0f));
        ImplicitObject *bottom = addObject(new ImplicitRotate(front, 270.0f, 0.0f, 0.0f));
        ImplicitObject *right = addObject(new ImplicitRotate(front, 0.0f, 90.0f, 0.0f));
        ImplicitObject *left = addObject(new ImplicitRotate(front, 0.0f, 270.0f, 0.0f));
        root->addBaseObject(cube);
        root->addBaseObject(front);
        root->addBaseObject(top);
        root->addBaseObject(back);
        root->addBaseObject(bottom);
        root->addBaseObject(right);
        root->addBaseObject(left);
        setRoot(root);
    }
};

class AstroidModel : public Model
{
public:
    AstroidModel() : Model() {
        NoiseGenerator *generatorLarge = addGenerator(new PerlinNoise());
        NoiseGenerator *generatorSmall = addGenerator(new PerlinNoise(0.2f, 3, 3.0f));
        ImplicitObject *cube = addObject(new ImplicitSphere(fieldfunctions::metaballFunction, Qt::gray, 1.5f));
        ImplicitObject *distorted = addObject(new ImplicitNoiseWarp(cube, generatorLarge, 0.7f));
        ImplicitObject *pockets = addObject(new ImplicitNoiseWarp(distorted, generatorSmall, -0.2f));
        ImplicitObject *coloured = addObject(new ImplicitNoiseColorizer(pockets, generatorSmall,
                                                                        QColor::fromRgbF(0.28f, 0.23f, 0.10f),
                                                                        QColor::fromRgbF(0.56f, 0.51f, 0.36f)));
        setRoot(coloured);
    }
};

class SnowflakeModel : public Model
{
public:
    SnowflakeModel(int depth) : Model() {
        ImplicitObject *base = addObject(new ImplicitLine(fieldfunctions::blinnFunction, Qt::white, 0.4f, 2.0f));
        ImplicitGroup *recursiveEdge = addGroup(new ImplicitBlendRicci(2.0, depth));
        ImplicitObject *offset = addObject(new ImplicitTranslate(recursiveEdge, 2.0f, 0.0f, 0.0f));
        ImplicitObject *scaled = addObject(new ImplicitScale(offset, 0.33f));
        ImplicitObject *right = addObject(new ImplicitRotate(scaled, 0.0f, 180.0f, 0.0f));
        ImplicitObject *tri1 = addObject(new ImplicitRotate(recursiveEdge, 0.0f, 0.0f, 60.0f));
        ImplicitObject *tri2 = addObject(new ImplicitTranslate(tri1, -0.5f, 0.86f, 0.0f));
        ImplicitObject *triLeft = addObject(new ImplicitScale(tri2, 0.33f));
        ImplicitObject *triRight = addObject(new ImplicitRotate(triLeft, 0.0f, 180.0f, 0.0f));
        ImplicitObject *triLeft2 = addObject(new ImplicitRotate(triLeft, 120.0f, 0.0f, 0.0f));
        ImplicitObject *triRight2 = addObject(new ImplicitRotate(triLeft, 120.0f, 180.0f, 0.0f));
        ImplicitObject *triLeft3 = addObject(new ImplicitRotate(triLeft, 240.0f, 0.0f, 0.0f));
        ImplicitObject *triRight3 = addObject(new ImplicitRotate(triLeft, 240.0f, 180.0f, 0.0f));
        recursiveEdge->addBaseObject(base);
        recursiveEdge->addRecursiveObject(scaled);
        recursiveEdge->addRecursiveObject(right);
        recursiveEdge->addRecursiveObject(triLeft);
        recursiveEdge->addRecursiveObject(triRight);
        recursiveEdge->addRecursiveObject(triLeft2);
        recursiveEdge->addRecursiveObject(triRight2);
        recursiveEdge->addRecursiveObject(triLeft3);
        recursiveEdge->addRecursiveObject(triRight3);
        setRoot(recursiveEdge);
    }
};

class SphereModel : public Model
{
public:
    SphereModel() : Model() {
        ImplicitObject *sphere = addObject(new ImplicitSphere(fieldfunctions::geoffFunction, 0.5f));
        ImplicitObject *scale = addObject(new ImplicitScale(sphere, 1.5f, 1.0f, 1.0f));
        ImplicitObject *offset = addObject(new ImplicitTranslate(scale, 0.0f, 0.5f, 0.0f));
        ImplicitGroup *blend = addGroup(new ImplicitBlend(0));
        blend->addBaseObject(offset);
        blend->addBaseObject(sphere);
        setRoot(blend);
    }
};

class RecursionModel : public Model
{
public:
    RecursionModel(int depth) : Model() {
        ImplicitObject *sphere = addObject(new ImplicitSphere(fieldfunctions::geoffFunction, 0.5f));
        ImplicitGroup *blend = addGroup(new ImplicitBlend(depth));
        ImplicitObject *offset = addObject(new ImplicitTranslate(blend, 0.0f, 0.6f, 0.0f));
        ImplicitObject *scale = addObject(new ImplicitScale(offset, 0.8));
        blend->addRecursiveObject(scale);
        blend->addRecursiveObject(sphere);
        blend->addBaseObject(sphere);
        setRoot(blend);
    }
};

class TableModel : public Model
{
public:
    TableModel() : Model() {
        ImplicitObject *cube = addObject(new ImplicitCube(fieldfunctions::wyvillFunction, 0.1f, 2.0f, 0.1f, 1.0f));
        ImplicitObject *line = addObject(new ImplicitLine(fieldfunctions::wyvillFunction, 0.1f, 0.5f));
        ImplicitObject *rotate = addObject(new ImplicitRotate(line, 0.0f, 0.0f, 90.0f));
        ImplicitObject *leg1 = addObject(new ImplicitTranslate(rotate, -0.9f, -0.35f, -0.4f));
        ImplicitObject *leg2 = addObject(new ImplicitTranslate(rotate, 0.9f, -0.35f, -0.4f));
        ImplicitObject *leg3 = addObject(new ImplicitTranslate(rotate, -0.9f, -0.35f, 0.4f));
        ImplicitObject *leg4 = addObject(new ImplicitTranslate(rotate, 0.9f, -0.35f, 0.4f));

        NoiseGenerator *woodGenerator = addGenerator(new GrainNoise());
        ImplicitGroup *table = addGroup(new ImplicitBlendRicci(5.0f, 0));
        table->addBaseObject(cube);
        table->addBaseObject(leg1);
        table->addBaseObject(leg2);
        table->addBaseObject(leg3);
        table->addBaseObject(leg4);
        ImplicitObject *color = addObject(new ImplicitNoiseShadeColorizer(table, woodGenerator, QColor(139, 69, 19)));
        setRoot(color);
    }
};

class ShapeTest : public Model
{
public:
    ShapeTest() : Model() {
        ImplicitObject *sphere = addObject(new ImplicitSphere(fieldfunctions::geoffFunction, 0.5f));
        ImplicitObject *transSphere = addObject(new ImplicitTranslate(sphere, -0.4f, 0.4f, 0.0f));
        ImplicitObject *line = addObject(new ImplicitLine(fieldfunctions::geoffFunction, 0.2f, 0.5f));
        ImplicitObject *transLine = addObject(new ImplicitTranslate(line, 0.4f, 0.4f, 0.0f));
        ImplicitObject *torus = addObject(new ImplicitTorus(fieldfunctions::geoffFunction, 0.2f, 0.5f));
        ImplicitObject *transTorus = addObject(new ImplicitTranslate(torus, -0.4f, -0.4f, 0.0f));
        ImplicitObject *cube = addObject(new ImplicitCube(fieldfunctions::geoffFunction, 0.2f, 0.4f));
        ImplicitObject *transCube = addObject(new ImplicitTranslate(cube, 0.4f, -0.4f, 0.0f));

        ImplicitGroup *root = addGroup(new ImplicitUnion(0));
        root->addBaseObject(transSphere);
        root->addBaseObject(transLine);
        root->addBaseObject(transTorus);
        root->addBaseObject(transCube);
        setRoot(root);
    }
};

class BlendTest : public Model
{
public:
    BlendTest() : Model() {
        ImplicitObject *line = addObject(new ImplicitLine(fieldfunctions::geoffFunction, 0.2f, 0.5f));
        ImplicitObject *line1 = addObject(new ImplicitRotate(line, 0.0f, 0.0f, 30.0f));
        ImplicitObject *line2 = addObject(new ImplicitRotate(line, 0.0f, 0.0f, -30.0f));

        ImplicitGroup *blend = addGroup(new ImplicitBlend(0));
        blend->addBaseObject(line1);
        blend->addBaseObject(line2);
        ImplicitObject *blendT = addObject(new ImplicitTranslate(blend, 0.0f, 0.3f, 0.0f));

        ImplicitGroup *ricci = addGroup(new ImplicitBlendRicci(4.0f, 0));
        ricci->addBaseObject(line1);
        ricci->addBaseObject(line2);
        ImplicitObject *ricciT = addObject(new ImplicitTranslate(ricci, 0.0f, -0.3f, 0.0f));

        ImplicitGroup *root = addGroup(new ImplicitUnion(0));
        root->addBaseObject(blendT);
        root->addBaseObject(ricciT);
        setRoot(root);
    }
};

class HueShiftTest : public Model
{
public:
    HueShiftTest() : Model() {
        ImplicitObject *sphere = addObject(new ImplicitSphere(fieldfunctions::wyvillFunction, 1.0f));
        ImplicitObject *shift = addObject(new ImplicitHueShiftColorizer(sphere, 0.4));
        ImplicitObject *translate = addObject(new ImplicitTranslate(shift, 0.0f, 1.0f, 0.0f));
        ImplicitGroup *uni = addGroup(new ImplicitUnion(0));
        uni->addBaseObject(sphere);
        uni->addBaseObject(translate);
        setRoot(uni);
    }
};

class CSGTest : public Model
{
public:
    CSGTest() : Model() {
        ImplicitObject *sphere = addObject(new ImplicitSphere(fieldfunctions::wyvillFunction, 0.5f));
        ImplicitObject *sphere2 = addObject(new ImplicitTranslate(sphere, 0.3f, 0.0f, 0.0f));

        ImplicitGroup *uni = addGroup(new ImplicitUnion(0));
        uni->addBaseObject(sphere);
        uni->addBaseObject(sphere2);
        ImplicitObject *uniT = addObject(new ImplicitTranslate(uni, 0.0f, 0.5f, 0.0f));

        ImplicitGroup *intersect = addGroup(new ImplicitIntersect(0));
        intersect->addBaseObject(sphere);
        intersect->addBaseObject(sphere2);

        ImplicitGroup *difference = addGroup(new ImplicitDifference(0.5f, 0));
        difference->addBaseObject(sphere);
        difference->addBaseObject(sphere2);
        ImplicitObject *differenceT = addObject(new ImplicitTranslate(difference, 0.0f, -0.5f, 0.0f));

        ImplicitGroup *blend = addGroup(new ImplicitUnion(0));
        blend->addBaseObject(uniT);
        blend->addBaseObject(intersect);
        blend->addBaseObject(differenceT);
        setRoot(blend);
    }
};

class IntersectionTest : public Model
{
public:
    IntersectionTest() : Model() {
        ImplicitObject *sphere1 = addObject(new ImplicitSphere(fieldfunctions::geoffFunction, Qt::red, 2.0f));
        ImplicitObject *sphere2 = addObject(new ImplicitSphere(fieldfunctions::geoffFunction, Qt::blue, 2.0f));
        ImplicitObject *sub1 = addObject(new ImplicitTranslate(sphere2, 0.f, -.6f, 0.0f));
        ImplicitGroup *root = addGroup(new ImplicitDifference(0.5f));
        root->addBaseObject(sphere1);
        root->addBaseObject(sub1);
        setRoot(root);
    }
};

class NoiseColorTest : public Model
{
public:
    NoiseColorTest() : Model() {
        NoiseGenerator *generator = addGenerator(new PerlinNoise());
        ImplicitObject *cube = addObject(new ImplicitCube(fieldfunctions::geoffFunction, Qt::blue, 0.5f, 1.0f));
        ImplicitObject *color = addObject(new ImplicitNoiseColorizer(cube, generator, Qt::yellow, Qt::blue));
        setRoot(color);
    }
};

class NoiseTest : public Model
{
public:
    NoiseTest() : Model() {
        NoiseGenerator *generator = addGenerator(new MarbleNoise());
        ImplicitObject *cube = addObject(new ImplicitSphere(fieldfunctions::metaballFunction, Qt::gray, 1.5f));
        ImplicitObject *distorted = addObject(new ImplicitNoiseWarp(cube, generator, 0.7f));
        setRoot(distorted);
    }
};

class TwistTest : public Model
{
public:
    TwistTest() : Model() {
        ImplicitObject *line = addObject(new ImplicitLine(fieldfunctions::geoffFunction, 0.5f, 2.0f));
        ImplicitObject *rotate = addObject(new ImplicitRotate(line, 0.0f, 90.0f, 0.0f));
        ImplicitObject *offset = addObject(new ImplicitTranslate(rotate, 0.0f, 0.5f, 0.0f));
        ImplicitObject *twist = addObject(new ImplicitTwist(offset, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f), 3.0f));
        setRoot(twist);
    }
};

class TaperTest : public Model
{
public:
    TaperTest() : Model() {
        ImplicitObject *line = addObject(new ImplicitLine(fieldfunctions::geoffFunction, 0.5f, 2.0f));
        ImplicitObject *rotate = addObject(new ImplicitRotate(line, 0.0f, 90.0f, 0.0f));
        ImplicitObject *taper = addObject(new ImplicitTaper(rotate, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f), -2.5f, 2.5f));
        setRoot(taper);
    }
};

class BendTest : public Model
{
public:
    BendTest() : Model() {
        ImplicitObject *line = addObject(new ImplicitLine(fieldfunctions::geoffFunction, 0.5f, 2.0f));
        //ImplicitObject *rotate = addObject(new ImplicitRotate(line, 0.0f, 0.0f, 90.0f));
        //ImplicitObject *offset = addObject(new ImplicitTranslate(rotate, 0.5f, 0.0f, 0.0f));
        ImplicitObject *bend = addObject(new ImplicitBend(line, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f), 0.5f));
        setRoot(bend);
    }
};

#endif // MODEL_H
