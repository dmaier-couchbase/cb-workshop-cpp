#pragma once
#include <QStringList>

struct QueryResult
{
    QStringList keys;
    int limit;
    int skip;
    int total;
};
