#-------------------------------------------------
#
# Project created by QtCreator 2014-04-02T11:36:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = git-annex-gui
TEMPLATE = app


SOURCES += main.cpp\
        repowindow.cpp \
    repository.cpp \
    remoteconfig.cpp \
    remotewanteddialog.cpp \
    remotewantededit.cpp \
    wantedexpr.cpp \
    slimlineedit.cpp \
    config_to_json.cpp \
    taglistwidget.cpp \
    taglistmodel.cpp \
    tagdelegate.cpp \
    repositorychooser.cpp \
    application.cpp

HEADERS  += repowindow.h \
    repository.h \
    remoteconfig.h \
    remotewanteddialog.h \
    remotewantededit.h \
    wantedexpr.h \
    slimlineedit.h \
    config_to_json.h \
    taglistwidget.h \
    taglistmodel.h \
    tagdelegate.h \
    repositorychooser.h \
    application.h

FORMS    += repowindow.ui \
    remoteconfig.ui \
    remotewanteddialog.ui \
    remotewantededit.ui \
    slimlineedit.ui \
    repositorychooser.ui

RESOURCES += \
    images.qrc
