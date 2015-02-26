#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QString>
#include <QMap>
#include <QListWidgetItem>

#include "polygonizerscene.h"
#include "model.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addModel(QString name, Model* model);

public slots:
    void drawOpenGL();
    void modelClicked(QListWidgetItem* item);
    void loadClicked();
    void voxelChanged(int size);
private:
    Ui::MainWindow *ui;
    QTimer* animationTimer;
    PolygonizerScene* m_scene;
    QMap<QString, Model*> m_models;

    QListWidgetItem *m_selected;
    float m_voxelSize;
};

#endif // MAINWINDOW_H
