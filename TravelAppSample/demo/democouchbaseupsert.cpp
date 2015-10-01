#include "democouchbaseupsert.h"
#include <qdebug.h>

void DemoCouchbaseUpsert::test()
{

    CBDataSource& ds = CBDataSourceFactory::Instance();
    ds.Upsert("test::hello", "{\"msg\"  : \"hello\"}");

    //TODO: Ask Mark why this is not supported.
    //ds.Upsert("test::hello", "{'msg'  : 'hello'}");

    QString doc =  ds.Get("test::hello").asString();
    qDebug() << doc;

    QJsonObject json;
    json.insert("msg", "hello again");
    ds.Upsert("test::hello::2", json);

    QString doc2 =  ds.Get("test::hello::2").asString();
    qDebug() << doc2;
}
