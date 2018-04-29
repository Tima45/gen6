#-------------------------------------------------
#
# Project created by QtCreator 2017-11-27T08:46:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = gen6
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QCUSTOMPLOT_USE_OPENGL

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += main.cpp\
        mainwindow.cpp \
    plot/qcustomplot.cpp \
    game.cpp \
    cell.cpp \
    empty.cpp \
    bot.cpp \
    doublecolors.cpp \
    worldparametersdialog.cpp

HEADERS  += mainwindow.h \
    plot/qcustomplot.h \
    game.h \
    cell.h \
    empty.h \
    bot.h \
    doublecolors.h \
    worldparametersdialog.h

FORMS    += mainwindow.ui \
    worldparametersdialog.ui

RC_FILE = icon.rc

RESOURCES += \
    pics.qrc

