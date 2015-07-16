#include "democouchbasemultiget.h"

void DemoCouchbaseMultiGet::test()
{
    CBDataSource& ds = CBDataSourceFactory::Instance();

    QStringList list;
    list.append("airline_19619");
    list.append("airline_19674");
    list.append("airline_19676");
    list.append("airline_19677");
    list.append("airline_19678");

    CouchbaseValueMap result = ds.MultiGet(list);

    for (CouchbaseValueMap::Iterator it = result.begin();it != result.end(); ++it)
    {
        qDebug() << "key = " + it.key();
        qDebug() <<  "value = " + it.value().asString();
    }
}
