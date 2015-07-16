#include "cbdatasourcefactory.h"

CBDataSource CBDataSourceFactory::mInstance;

void CBDataSourceFactory::Create(const QString &connectionString)
{
    mInstance(connectionString);
}

 CBDataSource& CBDataSourceFactory::Instance()
{
    return mInstance;
}
