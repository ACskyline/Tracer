#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),  ui(new Ui::MainWindow),
    mScene(Camera(Point3f(0,0.5,12), Point3f(0,0,0), Vector3f(0,1,0), 50.f, 0.1f, 1000.f, 400, 400))//this is an old version glm, it uses degree
{
    ui->setupUi(this);

    mScene.initializeScene();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSave_triggered()
{
    QImage result = mScene.renderScene();
    QPixmap pixmap(QPixmap::fromImage(result));

    graphics_scene.addPixmap(pixmap);
    graphics_scene.setSceneRect(pixmap.rect());
    ui->graphicsView->setScene(&graphics_scene);

    QString filename = QFileDialog::getSaveFileName(0, QString("Save Image"), QString("../.."), QString("*.bmp"));
    QString ext = filename.right(4);
    if(QString::compare(ext, QString(".bmp")) != 0)
    {
        filename.append(QString(".bmp"));
    }
    QImageWriter writer(filename);
    writer.setFormat("bmp");
    if(!writer.write(result))
    {
        qDebug() << writer.errorString();
    }
}

void MainWindow::on_actionJust_Show_It_triggered()
{
    QImage result = mScene.renderScene();
    QPixmap pixmap(QPixmap::fromImage(result));

    graphics_scene.addPixmap(pixmap);
    graphics_scene.setSceneRect(pixmap.rect());
    ui->graphicsView->setScene(&graphics_scene);
}
