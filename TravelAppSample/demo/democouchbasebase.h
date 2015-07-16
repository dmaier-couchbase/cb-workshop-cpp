#ifndef DEMOCOUCHBASEBASE_H
#define DEMOCOUCHBASEBASE_H

#include <libcouchbase/couchbase.h>
#include "cbdatasource.h"
#include "cbdatasourcefactory.h"
#include <qdebug.h>

class DemoCouchbaseBase
{
public:
    void virtual test() = 0;
};

#endif // DEMOCOUCHBASEBASE_H
