#-------------------------------------------------
#
# Project created by QtCreator 2014-07-07T20:18:14
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FinalProject
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    openglview.cpp \
    polygonizerscene.cpp \
    fieldfunctions.cpp \
    implicitsphere.cpp \
    polygonizer.cpp \
    polygonizertables.cpp \
    implicitmodel.cpp \
    implicittransform.cpp \
    implicitgroup.cpp \
    implicitline.cpp \
    implicittorus.cpp \
    orbitallightsource.cpp \
    perlinnoise.cpp \
    implicitcolorizer.cpp \
    implicitcube.cpp \
    implicitplane.cpp \
    implicitnoisewarp.cpp \
    implicitwarp.cpp

HEADERS  += mainwindow.h \
    openglview.h \
    polygonizerscene.h \
    fieldfunctions.h \
    polygonizervoxel.h \
    implicitobject.h \
    implicitsphere.h \
    polygonizer.h \
    implicitmodel.h \
    implicittransform.h \
    implicitgroup.h \
    implicitline.h \
    implicittorus.h \
    orbitallightsource.h \
    perlinnoise.h \
    implicitcolorizer.h \
    model.h \
    implicitcube.h \
    implicitplane.h \
    implicitnoisewarp.h \
    implicitwarp.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    Basic.fsh \
    Basic.vsh

INSTALLS += OTHER_FILES
