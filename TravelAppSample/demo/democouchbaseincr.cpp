#include "democouchbaseincr.h"


void DemoCouchbaseIncr::test()
{

    CBDataSource& ds = CBDataSourceFactory::Instance();

    ds.IncrCounter("test::counter",1,0);

    QString init = ds.Get("test::counter");
    qDebug() << "Initialized Counter: " + init;


    for (int var = 0; var < 10; ++var) {

        ds.IncrCounter("test::counter", 1);

    }

    QString final = ds.Get("test::counter");
    qDebug() << "Final Counter Value is: " + final;

}
