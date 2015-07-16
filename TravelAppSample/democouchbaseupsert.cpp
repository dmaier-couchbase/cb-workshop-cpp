#include "democouchbaseupsert.h"
#include <qdebug.h>

void DemoCouchbaseUpsert::test()
{

    CBDataSource& ds = CBDataSourceFactory::Instance();
    ds.Upsert("test::hello", "{\"msg\"  : \"hello\"}");

    //TODO: Ask Mark why this is not supported.
    //ds.Upsert("test::hello", "{'msg'  : 'hello'}");

    QString doc =  ds.Get("test::hello");
    qDebug() << doc;
}
