#pragma once

#include "cbqueryresult.h"
#include "cbn1qlresult.h"
#include "couchbasedocument.h"

#include <QString>
#include <QJsonObject>

struct lcb_st;
typedef struct lcb_st *lcb_t;

class CBDataSource
{
public:
    CBDataSource();
    void Destroy();

    void Connect(const QString &connectionString);

    CouchbaseDocument Get(QString key);
    bool Upsert(QString key, QString document);
    bool Upsert(QString key, QJsonObject document);
    bool Delete(QString key);
    bool IncrCounter(QString name, int delta, int initial = 0);
    CouchbaseDocumentMap MultiGet(QStringList keys);
    CBQueryResult QueryView(QString designDocName, QString viewName, int limit=0, int skip=0);
    CBN1qlResult QueryN1ql(QString query);

private:
    lcb_t mInstance;
    bool mIsConnected;
};
