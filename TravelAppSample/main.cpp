#include <QApplication>
#include <stdio.h>
#include "mainwindow.h"
#include "democouchbaseconnect.h"
#include "democouchbaseget.h"
#include "democouchbaseupsert.h"
#include "democouchbasedelete.h"
#include "democouchbaseincr.h"
#include "democouchbasemultiget.h"

int main(int argc, char *argv[])
{

    printf("Starting demo application ...\n");

    //Demos/Test
    DemoCouchbaseConnect connectDemo;
    connectDemo.test();

    DemoCouchbaseGet getDemo;
    getDemo.test();
    
    DemoCouchbaseUpsert upsertDemo;
    upsertDemo.test();
    
    DemoCouchbaseDelete deleteDemo;
    deleteDemo.test();

    DemoCouchbaseIncr incrDemo;
    incrDemo.test();

    DemoCouchbaseMultiGet multiGetDemo;
    multiGetDemo.test();

    //Window
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
