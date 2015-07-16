#include "democouchbaseconnect.h"


void DemoCouchbaseConnect::test()
{
   CBDataSourceFactory::Create("couchbase://localhost/travel-sample");
}
