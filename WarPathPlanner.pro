#-------------------------------------------------
#
# Project created by QtCreator 2011-01-29T17:13:15
#
#-------------------------------------------------

QT       += core gui

TARGET = WarPathPlanner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    treemodel.cpp \
    treeitem.cpp \
    sharedmodel.cpp \
    shareditem.cpp \
    associatedunitsmodel.cpp \
    filelistwindow.cpp \
    filelisttreemodel.cpp \
    closeconfirm.cpp

HEADERS  += mainwindow.h \
    treemodel.h \
    treeitem.h \
    sharedmodel.h \
    shareditem.h \
    associatedunitsmodel.h \
    filelistwindow.h \
    filelisttreemodel.h \
    closeconfirm.h

FORMS    += mainwindow.ui \
    filelistwindow.ui \
    closeconfirm.ui

OTHER_FILES +=

RESOURCES += \
    WarPathPlanner.qrc
