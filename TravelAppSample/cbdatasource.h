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
    void Upsert(QString key, QJsonObject document);
    bool Delete(QString key);
    bool UpdateCounter(QString name, int delta, int initial);

    QueryResult QueryView(int limit = 0, int skip = 0);

private:
    lcb_t mInstance;
    bool mIsConnected;
};
