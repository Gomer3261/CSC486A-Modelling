#ifndef OPENGLVIEW_H
#define OPENGLVIEW_H

//#include "glu.h"
#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMatrix4x4>
#include <QGLShaderProgram>
#include <QGLShader>
#include <QDebug>
#include "polygonizerscene.h"

class OpenGLView : public QGLWidget
{
    Q_OBJECT
public:
    explicit OpenGLView(QWidget *parent = 0);
    void setScene(PolygonizerScene* scene);
signals:

public slots:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void updateCameraAngle();
    void loadShader(QString vshader, QString fshader);

    PolygonizerScene* m_scene;
private:
    float   screenWidth;
    float   screenHeight;
    float   aspectRatio;
    float   nearPlane;
    float   farPlane;
    float   fieldOfView;

    QMatrix4x4  m_projectionMatrix;
    QMatrix4x4  m_viewMatrix;
    QMatrix4x4  m_invViewMatrix;
    QVector3D   m_cameraPosition;

    QGLShaderProgram *m_shaderProgram;
    QGLShader *m_vertexShader;
    QGLShader *m_fragmentShader;

    float       m_rotationX;
    float       m_rotationY;
    float       m_distance;
    int         m_screenPreviousX;
    int         m_screenPreviousY;
};

#endif // OPENGLVIEW_H
