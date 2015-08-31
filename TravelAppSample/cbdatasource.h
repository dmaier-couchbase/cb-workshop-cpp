#pragma once

#include "queryresult.h"
#include "n1clresult.h"
#include "couchbasevalue.h"

#include <QString>
#include <QJsonObject>

struct lcb_st;
typedef struct lcb_st *lcb_t;

typedef QMap<QString, CouchbaseValue> CouchbaseValueMap;

class CBDataSource
{
public:
    CBDataSource();
    void Destroy();

    void Connect(const QString &connectionString);

    QString Get(QString key);
    QJsonObject GetJsonObject(QString key);
    bool Upsert(QString key, QString document);
    bool Upsert(QString key, QJsonObject document);
    bool Delete(QString key);
    bool IncrCounter(QString name, int delta, int initial = 0);
    CouchbaseValueMap MultiGet(QStringList keys);
    QueryResult QueryView(QString designDocName, QString viewName, int limit=0, int skip=0);
    N1clResult QueryN1cl(QString query);

private:
    lcb_t mInstance;
    bool mIsConnected;
};
