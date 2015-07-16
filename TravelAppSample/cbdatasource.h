#pragma once

#include "queryresult.h"
#include <QString>
#include <QJsonObject>

struct lcb_st;
typedef struct lcb_st *lcb_t;

class CBDataSource
{
public:
    CBDataSource();
    ~CBDataSource();

    void Connect(const QString &connectionString);

    QString Get(QString key);
    QJsonObject GetJsonObject(QString key);
    void Upsert(QString key, QString document);
    QueryResult QueryView(int limit = 0, int skip = 0);

private:
    lcb_t mInstance;
    bool mIsConnected;
};
