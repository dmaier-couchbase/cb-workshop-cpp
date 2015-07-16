#include "cbdatasourcefactory.h"

CBDataSource CBDataSourceFactory::mInstance;

void CBDataSourceFactory::Create(const QString &connectionString)
{
    mInstance.Connect(connectionString);
}

CBDataSource& CBDataSourceFactory::Instance()
{
    return mInstance;
}
