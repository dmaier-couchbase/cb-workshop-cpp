#include "mainwindow.h"
#include <QApplication>
#include <democouchbaseconnect.h>
#include <democouchbaseget.h>
#include <stdio.h>

int main(int argc, char *argv[])
{

    printf("Starting demo application ...\n");

    //Demos/Test
    DemoCouchbaseConnect connectDemo;
    connectDemo.test();

    DemoCouchbaseGet getDemo;
    getDemo.test();
    
    

    //Window
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
