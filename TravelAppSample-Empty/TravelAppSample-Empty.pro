#-------------------------------------------------
#
# Project created by QtCreator 2015-07-14T13:36:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TravelAppSample-Empty
TEMPLATE = app


SOURCES += main.cpp\
    demo/democouchbaseconnect.cpp \
    cbdatasource.cpp \
    demo/democouchbaseget.cpp \
    demo/democouchbaseupsert.cpp \
    cbdatasourcefactory.cpp \
    demo/democouchbasedelete.cpp \
    demo/democouchbaseincr.cpp \
    couchbasevalue.cpp \
    demo/democouchbasemultiget.cpp \
    demo/democouchbaseview.cpp \
    demo/democouchbasen1ql.cpp \
    mainwindow.cpp

HEADERS  += mainwindow.h \
    demo/democouchbaseconnect.h \
    cbdatasource.h \
    queryresult.h \
    demo/democouchbaseget.h \
    demo/democouchbasebase.h \
    demo/democouchbaseupsert.h \
    cbdatasourcefactory.h \
    demo/democouchbasedelete.h \
    demo/democouchbaseincr.h \
    couchbasevalue.h \
    multiget.h \
    demo/democouchbasemultiget.h \
    demo/democouchbaseview.h \
    login.h \
    usermodel.h \
    n1qlresult.h \
    demo/democouchbasen1ql.h \
    mainwindow.h

FORMS    += \
    mainwindow.ui

## Couchbase Options

# Header file
unix::INCLUDEPATH += /usr/include/libcouchbase

# Linker option -l couchbase
unix::LIBS  = -L/usr/lib64 -lcouchbase


win32::LIBS += -L$$PWD/../../couchbase/libcouchbase-2.5.0_amd64_vc11/lib/ -llibcouchbase_d
win32::INCLUDEPATH += $$PWD/../../couchbase/libcouchbase-2.5.0_amd64_vc11/include





