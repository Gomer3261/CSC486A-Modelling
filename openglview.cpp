#include "openglview.h"
#include <QFileInfo>

OpenGLView::OpenGLView(QWidget *parent)  :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    m_scene(NULL),
    m_projectionMatrix(),
    m_viewMatrix(),
    m_cameraPosition(),
    m_shaderProgram(NULL),
    m_vertexShader(NULL),
    m_fragmentShader(NULL),
    m_rotationX(20),
    m_rotationY(0),
    m_distance(4.0f)
{
    screenWidth = 0.0f;
    screenHeight = 0.0f;
    aspectRatio = 1.0f;
    fieldOfView = 60.0f;
    nearPlane = 0.01f;
    farPlane = 50.0f;

    updateCameraAngle();
}

void OpenGLView::setScene(PolygonizerScene* scene) {
    m_scene = scene;
    m_scene->initializeGL();
}

void OpenGLView::updateCameraAngle() {
    QMatrix4x4 rotx;
    rotx.rotate(m_rotationX, 0.0f, 1.0f, 0.0f);
    QMatrix4x4 roty;
    roty.rotate(m_rotationY, 1.0f, 0.0f, 0.0f);
    m_cameraPosition = rotx * roty * QVector3D(0.0f, 0.0f, -m_distance);
}

void OpenGLView::initializeGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    loadShader("./Basic.vsh", "./Basic.fsh");
}

void OpenGLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glMatrixMode(GL_MODELVIEW);
    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(m_cameraPosition,
                      QVector3D(0.0f, 0.0f, 0.0f),
                      QVector3D(0.0f, 1.0f, 0.0f));
    glLoadMatrixf(m_viewMatrix.constData());

    m_scene->paintGL();
}

void OpenGLView::resizeGL(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    aspectRatio = screenWidth/screenHeight;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
    glLoadMatrixf(m_projectionMatrix.constData());

    glMatrixMode(GL_MODELVIEW);
}

void OpenGLView::mousePressEvent(QMouseEvent *event)
{
    m_screenPreviousX = event->pos().x();
    m_screenPreviousY = event->pos().y();

    event->accept();
    QGLWidget::mousePressEvent(event);
}

void OpenGLView::mouseMoveEvent(QMouseEvent *event)
{
    m_rotationX += (m_screenPreviousX - event->pos().x())/1.0f;
    m_rotationY -= (m_screenPreviousY - event->pos().y())/1.0f;

    if(m_rotationX > 360.0f) m_rotationX -= 360.0f;
    if(m_rotationX < 0.0f) m_rotationX += 360.0f;
    if(m_rotationY > 70.0f) m_rotationY = 70.0f;
    if(m_rotationY < -70.0f) m_rotationY = -70.0f;

    m_screenPreviousX = event->pos().x();
    m_screenPreviousY = event->pos().y();

    updateCameraAngle();

    event->accept();
    QGLWidget::mouseMoveEvent(event);
}

void OpenGLView::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    QGLWidget::mouseReleaseEvent(event);
}

void OpenGLView::wheelEvent(QWheelEvent *event)
{
    m_distance = m_distance + event->delta()*0.02f;
    if(m_distance < 2.0f) m_distance = 2.0f;
    if(m_distance > 10.0f) m_distance = 10.0f;
    updateCameraAngle();
}

void OpenGLView::loadShader(QString vshader, QString fshader)
{
    qDebug() << "Loading Shaders";
    if(m_shaderProgram) {
        m_shaderProgram->release();
        m_shaderProgram->removeAllShaders();
    } else {
        m_shaderProgram = new QGLShaderProgram;
    }

    if(m_vertexShader) {
        delete m_vertexShader;
        m_vertexShader = NULL;
    }

    if(m_fragmentShader) {
        delete m_fragmentShader;
        m_fragmentShader = NULL;
    }

    // load and compile vertex shader
    QFileInfo vsh(vshader);
    if(vsh.exists()) {
        m_vertexShader = new QGLShader(QGLShader::Vertex);
        if(m_vertexShader->compileSourceFile(vshader)) {
            m_shaderProgram->addShader(m_vertexShader);
        } else qWarning() << "Vertex Shader Error" << m_vertexShader->log();
    } else qWarning() << "Vertex Shader source file " << vshader << " does not exist";

    //load and compile fragment shader
    QFileInfo fsh(fshader);
    if(fsh.exists()) {
        m_fragmentShader = new QGLShader(QGLShader::Fragment);
        if(m_fragmentShader->compileSourceFile(fshader)) {
                m_shaderProgram->addShader(m_fragmentShader);
        } else qWarning() << "Vertex Shader Error " << m_fragmentShader->log();
    } else qWarning() << "Vertex Shader source file " << fshader << " does not exist";

    if(!m_shaderProgram->link()) {
        qWarning() << "Shader program linker error" << m_shaderProgram->log();
    }
    else m_shaderProgram->bind(); // Send the program to the graphics card.
}
