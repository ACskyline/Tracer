/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "mygl.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionLoad_Scene;
    QAction *actionRender;
    QAction *actionCamera_Controls;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    MyGL *mygl;
    QGroupBox *groupBox;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QSpinBox *samplesSpinBox;
    QComboBox *integratorSpinBox;
    QLabel *label_2;
    QLabel *label_3;
    QSpinBox *recursionSpinBox;
    QLabel *label_4;
    QSpinBox *photonSpinBox;
    QDoubleSpinBox *doubleSpinBox;
    QLabel *label_5;
    QCheckBox *checkBox_Progressive;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(655, 610);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionLoad_Scene = new QAction(MainWindow);
        actionLoad_Scene->setObjectName(QStringLiteral("actionLoad_Scene"));
        actionRender = new QAction(MainWindow);
        actionRender->setObjectName(QStringLiteral("actionRender"));
        actionCamera_Controls = new QAction(MainWindow);
        actionCamera_Controls->setObjectName(QStringLiteral("actionCamera_Controls"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        mygl = new MyGL(centralWidget);
        mygl->setObjectName(QStringLiteral("mygl"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(mygl->sizePolicy().hasHeightForWidth());
        mygl->setSizePolicy(sizePolicy);
        mygl->setMinimumSize(QSize(618, 432));

        verticalLayout->addWidget(mygl);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        groupBox->setMinimumSize(QSize(631, 121));
        groupBox->setBaseSize(QSize(631, 121));
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 20, 611, 91));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setWordWrap(true);

        gridLayout->addWidget(label, 0, 0, 1, 2);

        samplesSpinBox = new QSpinBox(layoutWidget);
        samplesSpinBox->setObjectName(QStringLiteral("samplesSpinBox"));
        samplesSpinBox->setMinimum(1);
        samplesSpinBox->setMaximum(100);
        samplesSpinBox->setValue(10);

        gridLayout->addWidget(samplesSpinBox, 0, 2, 1, 1);

        integratorSpinBox = new QComboBox(layoutWidget);
        integratorSpinBox->setObjectName(QStringLiteral("integratorSpinBox"));

        gridLayout->addWidget(integratorSpinBox, 1, 1, 1, 2);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setWordWrap(true);

        gridLayout->addWidget(label_2, 0, 3, 1, 1);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setWordWrap(true);

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        recursionSpinBox = new QSpinBox(layoutWidget);
        recursionSpinBox->setObjectName(QStringLiteral("recursionSpinBox"));
        recursionSpinBox->setMinimum(1);
        recursionSpinBox->setMaximum(100);
        recursionSpinBox->setValue(5);

        gridLayout->addWidget(recursionSpinBox, 0, 4, 1, 1);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setWordWrap(true);

        gridLayout->addWidget(label_4, 1, 3, 1, 1);

        photonSpinBox = new QSpinBox(layoutWidget);
        photonSpinBox->setObjectName(QStringLiteral("photonSpinBox"));
        photonSpinBox->setMinimum(10000);
        photonSpinBox->setMaximum(999999999);
        photonSpinBox->setSingleStep(1000);
        photonSpinBox->setValue(100000);

        gridLayout->addWidget(photonSpinBox, 1, 4, 1, 1);

        doubleSpinBox = new QDoubleSpinBox(layoutWidget);
        doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));
        doubleSpinBox->setDecimals(4);
        doubleSpinBox->setMinimum(0.001);
        doubleSpinBox->setMaximum(2);
        doubleSpinBox->setValue(0.01);

        gridLayout->addWidget(doubleSpinBox, 1, 6, 1, 1);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 1, 5, 1, 1);

        checkBox_Progressive = new QCheckBox(layoutWidget);
        checkBox_Progressive->setObjectName(QStringLiteral("checkBox_Progressive"));
        checkBox_Progressive->setChecked(true);

        gridLayout->addWidget(checkBox_Progressive, 0, 5, 1, 2);


        verticalLayout->addWidget(groupBox);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 655, 20));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionRender);
        menuFile->addAction(actionLoad_Scene);
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionCamera_Controls);

        retranslateUi(MainWindow);
        QObject::connect(mygl, SIGNAL(sig_ResizeToCamera(int,int)), MainWindow, SLOT(slot_ResizeToCamera(int,int)));
        QObject::connect(samplesSpinBox, SIGNAL(valueChanged(int)), mygl, SLOT(slot_SetNumSamplesSqrt(int)));
        QObject::connect(recursionSpinBox, SIGNAL(valueChanged(int)), mygl, SLOT(slot_SetRecursionLimit(int)));
        QObject::connect(checkBox_Progressive, SIGNAL(toggled(bool)), mygl, SLOT(slot_SetProgressiveRender(bool)));
        QObject::connect(integratorSpinBox, SIGNAL(currentIndexChanged(int)), mygl, SLOT(slot_SetIntegratorType(int)));
        QObject::connect(mygl, SIGNAL(sig_DisableGUI(bool)), MainWindow, SLOT(slot_DisableGUI(bool)));
        QObject::connect(photonSpinBox, SIGNAL(valueChanged(int)), mygl, SLOT(slot_SetPhotonCount(int)));
        QObject::connect(doubleSpinBox, SIGNAL(valueChanged(double)), mygl, SLOT(slot_SetSearchRadius(double)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Path Tracer", 0));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0));
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0));
        actionLoad_Scene->setText(QApplication::translate("MainWindow", "Load Scene", 0));
        actionLoad_Scene->setShortcut(QApplication::translate("MainWindow", "Ctrl+L", 0));
        actionRender->setText(QApplication::translate("MainWindow", "Render", 0));
        actionRender->setShortcut(QApplication::translate("MainWindow", "Ctrl+R", 0));
        actionCamera_Controls->setText(QApplication::translate("MainWindow", "Camera Controls", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Controls", 0));
        label->setText(QApplication::translate("MainWindow", "Square root of pixel samples count:", 0));
        integratorSpinBox->clear();
        integratorSpinBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Naive", 0)
         << QApplication::translate("MainWindow", "Direct Lighting", 0)
         << QApplication::translate("MainWindow", "Indirect Lighting", 0)
         << QApplication::translate("MainWindow", "Full Lighting", 0)
         << QApplication::translate("MainWindow", "Photon Mapping", 0)
        );
        label_2->setText(QApplication::translate("MainWindow", "Recursion limit:", 0));
        label_3->setText(QApplication::translate("MainWindow", "Integrator type:", 0));
        label_4->setText(QApplication::translate("MainWindow", "Photon count:", 0));
        label_5->setText(QApplication::translate("MainWindow", "Search radius", 0));
        checkBox_Progressive->setText(QApplication::translate("MainWindow", "Progressive Preview", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
