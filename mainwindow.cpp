#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_selected(NULL)
{
    ui->setupUi(this);
    animationTimer = new QTimer(this);

    connect(animationTimer, SIGNAL(timeout()), this, SLOT(drawOpenGL()));
    animationTimer->start(20);

    addModel(QString("Simple Model"), new SphereModel());
    addModel(QString("Recursion Model Depth:1"), new RecursionModel(1));
    addModel(QString("Recursion Model Depth:4"), new RecursionModel(4));
    addModel(QString("Snowman"), new SnowmanModel(2));
    addModel(QString("Tall Snowman"), new SnowmanModel(5));
    addModel(QString("StarRay Depth:0"), new SnowflakeModel(0));
    addModel(QString("StarRay Depth:1"), new SnowflakeModel(1));
    addModel(QString("StarRay Depth:2"), new SnowflakeModel(2));
    addModel(QString("StarRay Depth:3"), new SnowflakeModel(3));
    addModel(QString("Rainbow"), new RainbowModel());
    addModel(QString("Cut Cube"), new CutCubeModel(2));
    addModel(QString("Astroid"), new AstroidModel());
    addModel(QString("Table"), new TableModel());
    addModel(QString("Shapes"), new ShapeTest());
    addModel(QString("Blend Test"), new BlendTest());
    addModel(QString("Hue Shift Test"), new HueShiftTest());
    addModel(QString("CSG Test"), new CSGTest());
    addModel(QString("Intersection Test"), new IntersectionTest());
    addModel(QString("Noise Color Test"), new NoiseColorTest());
    addModel(QString("Noise Warp Test"), new NoiseTest());
    addModel(QString("Twist test"), new TwistTest());
    addModel(QString("Taper test"), new TaperTest());
    addModel(QString("Bend test"), new BendTest());

    m_scene = new PolygonizerScene();
    m_scene->setModel(m_models[QString("StarRay Depth:3")]);

    voxelChanged(ui->voxelSlider->value());
    ui->oglview->setScene(m_scene);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_scene;
    QMapIterator<QString, Model*> modelIter(m_models);
    while (modelIter.hasNext()) {
        modelIter.next();
        delete modelIter.value();
    }
}

void MainWindow::addModel(QString name, Model* model) {
    m_models.insert(name, model);
    new QListWidgetItem(name, ui->modelList);
}

void MainWindow::drawOpenGL()
{
    ui->oglview->repaint();
}

void MainWindow::modelClicked(QListWidgetItem* item) {
    m_selected = item;
}

void MainWindow::loadClicked() {
    if(m_selected != NULL && m_models.contains(m_selected->text())) {
        m_scene->setModel(m_models[m_selected->text()]);
    }
}

void MainWindow::voxelChanged(int size) {
    float vSize = 1.0f / (size*0.1f);
    if(vSize != m_voxelSize) {
        m_voxelSize = vSize;
        m_scene->setVoxelSize(m_voxelSize);
    }
}
