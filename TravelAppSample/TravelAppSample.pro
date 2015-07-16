#-------------------------------------------------
#
# Project created by QtCreator 2015-07-14T13:36:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TravelAppSample
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    democouchbaseconnect.cpp \
    cbdatasource.cpp \
    democouchbaseget.cpp \
    democouchbaseupsert.cpp \
    cbdatasourcefactory.cpp

HEADERS  += mainwindow.h \
    democouchbaseconnect.h \
    cbdatasource.h \
    queryresult.h \
    democouchbaseget.h \
    democouchbasebase.h \
    democouchbaseupsert.h \
    cbdatasourcefactory.h

FORMS    += mainwindow.ui

## Couchbase Options
unix
{
# Header file
INCLUDEPATH += /usr/include/libcouchbase

# Linker option -l couchbase
LIBS  = -L/usr/lib64 -lcouchbase
}

win32
{
LIBS += -L$$PWD/../../couchbase/libcouchbase-2.5.0_amd64_vc11/lib/ -llibcouchbase_d
INCLUDEPATH += $$PWD/../../couchbase/libcouchbase-2.5.0_amd64_vc11/include
}




