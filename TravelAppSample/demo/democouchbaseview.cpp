#include "democouchbaseview.h"

void DemoCouchbaseView::test()
{
    CBDataSource& ds = CBDataSourceFactory::Instance();

    QueryResult  result = ds.QueryView("airports", "by_name", 10);

}
