#pragma once
#include <QStringList>

struct N1clResult
{
    QList<QJsonObject> items;
    int limit;
    int skip;
    int total;
};
