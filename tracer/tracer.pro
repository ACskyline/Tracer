#-------------------------------------------------
#
# Project created by QtCreator 2018-01-22T19:44:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tracer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ray.cpp \
    camera.cpp \
    intersection.cpp \
    transform.cpp \
    shape.cpp \
    primitive.cpp \
    scene.cpp \
    sphere.cpp \
    squareplane.cpp \
    light.cpp \
    pointlight.cpp \
    material.cpp \
    lambertmaterial.cpp \
    blinnphongmaterial.cpp \
    specularreflectionmaterial.cpp \
    speculartransmissionmaterial.cpp \
    sampler.cpp \
    warpfunctions.cpp \
    disk.cpp \
    arealight.cpp

HEADERS  += mainwindow.h \
    globalincludes.h \
    ray.h \
    camera.h \
    intersection.h \
    transform.h \
    shape.h \
    primitive.h \
    scene.h \
    sphere.h \
    squareplane.h \
    light.h \
    pointlight.h \
    material.h \
    lambertmaterial.h \
    blinnphongmaterial.h \
    specularreflectionmaterial.h \
    speculartransmissionmaterial.h \
    sampler.h \
    warpfunctions.h \
    pcg32.h \
    enums.h \
    disk.h \
    arealight.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/include

DEPENDPATH += $$PWD/include

CONFIG += c++11
