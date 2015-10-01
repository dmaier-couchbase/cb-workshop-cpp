#include "democouchbaseget.h"
#include <qdebug.h>

void DemoCouchbaseGet::test()
{
    //TODO: Support bucket name and password as params
    CBDataSource& ds = CBDataSourceFactory::Instance();

    QString content = ds.Get("route_52351").asString();

    qDebug() << "Got the following content for document with key route_52351:" ;
    qDebug() << content;

    QJsonObject obj =  ds.Get("airline_10").asJson();
    QJsonValue val = obj.value("name");

    qDebug() << "Got the following name value for document with key airline_10:";
    qDebug() << val.toString();

}
