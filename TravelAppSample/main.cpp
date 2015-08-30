#include <QApplication>
#include <stdio.h>
#include "mainwindow.h"
#include "demo/democouchbaseconnect.h"
#include "demo/democouchbaseget.h"
#include "demo/democouchbaseupsert.h"
#include "demo/democouchbasedelete.h"
#include "demo/democouchbaseincr.h"
#include "demo/democouchbasemultiget.h"
#include "demo/democouchbaseview.h"

int main(int argc, char *argv[])
{

    printf("Starting demo application ...\n");

    CBDataSourceFactory::Create("couchbase://localhost/travel-sample");

    //Demos/Test
//    DemoCouchbaseConnect connectDemo;
//    connectDemo.test();

//    DemoCouchbaseGet getDemo;
//    getDemo.test();
    
//    DemoCouchbaseUpsert upsertDemo;
//    upsertDemo.test();
    
//    DemoCouchbaseDelete deleteDemo;
//    deleteDemo.test();

//    DemoCouchbaseIncr incrDemo;
//    incrDemo.test();

//    DemoCouchbaseMultiGet multiGetDemo;
//    multiGetDemo.test();

//    DemoCouchbaseView viewDemo;
//    viewDemo.test();

    //Window
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
