
#include "democouchbasen1ql.h"

void DemoCouchbaseN1ql::test()
{
    CBDataSource& ds = CBDataSourceFactory::Instance();

    QString queryPrep = "SELECT airportname FROM `travel-sample` WHERE faa = 'LHR'";

    CBN1qlResult result = ds.QueryN1ql(queryPrep);

    for (QList<QJsonObject>::Iterator it = result.items.begin();it != result.items.end(); ++it)
    {
        qDebug() << "result = " << (*it)["airportname"].toString();
    }
}
