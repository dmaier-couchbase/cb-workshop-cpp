#pragma once
#include <QStringList>

struct QueryResultEntry
{
    QString key;
    QString value;
};

struct QueryResult
{
    QList<QueryResultEntry> items;
    int limit;
    int skip;
    int total;
};
