
#include "democouchbasen1cl.h"

void DemoCouchbaseN1cl::test()
{
    CBDataSource& ds = CBDataSourceFactory::Instance();

    QString queryPrep = "SELECT airportname FROM `travel-sample` WHERE faa = 'LHR'";

    N1clResult result = ds.QueryN1cl(queryPrep);

    for (QList<QJsonObject>::Iterator it = result.items.begin();it != result.items.end(); ++it)
    {
        qDebug() << "result = " << (*it)["airportname"].toString();
    }
}
