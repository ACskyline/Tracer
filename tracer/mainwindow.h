#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QImageWriter>
#include <QFileDialog>
#include <QDebug>
#include <QGraphicsScene>
#include "scene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionSave_triggered();

    void on_actionJust_Show_It_triggered();

private:
    Ui::MainWindow *ui;

    QGraphicsScene graphics_scene;

    Scene mScene;
};

#endif // MAINWINDOW_H
