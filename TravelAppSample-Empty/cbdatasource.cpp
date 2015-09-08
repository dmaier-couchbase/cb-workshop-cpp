#include "cbdatasource.h"
#include "multiget.h"

#include <libcouchbase/couchbase.h>
#include <libcouchbase/views.h>
#include <libcouchbase/n1ql.h>
#include <stdio.h>

#include <QDebug>
#include <QStringList>
#include <QJsonObject>
#include <QJsonDocument>
#include <QException>

static void
storage_callback(lcb_t instance, const void *cookie, lcb_storage_t op, lcb_error_t err,
                 const lcb_store_resp_t *resp)
{
    if (err != LCB_SUCCESS)
    {
        qDebug() << QString("Couldn't store item to cluster: %1").arg(QString::fromUtf8(lcb_strerror(instance, err)));
    }
}

static QString result;
static bool multiGet = false;
static lcb_cas_t cas;

static void
get_callback(lcb_t instance, const void *cookie, lcb_error_t err, const lcb_get_resp_t *resp)
{
    if (multiGet)
    {
        MultiGetResult *mg = const_cast<MultiGetResult*>(reinterpret_cast<const MultiGetResult*>(cookie));
        mg->handleResponse(resp, err);
    }
    else
    {
        cas = resp->v.v0.cas;
        QByteArray ba = QByteArray((const char*)resp->v.v0.bytes, resp->v.v0.nbytes);
        result = QString(ba);
    }
}

static bool removedSuccessfully;

static void
on_removed(lcb_t instance, const void *cookie, lcb_error_t err, const lcb_remove_resp_t *resp)
{
    if (err != LCB_SUCCESS)
    {
        qDebug() << QString("Failed to remove item: %1").arg(QString::fromUtf8(lcb_strerror(instance, err)));
        removedSuccessfully = false;
    }
    else
    {
        removedSuccessfully = true;
    }
}

CBDataSource::CBDataSource()
    : mIsConnected(false)
{
}

void CBDataSource::Connect(const QString &connectionString)
{
    //TODO: Excercise 7a
}

void CBDataSource::Destroy()
{
    if (mIsConnected && mInstance != NULL)
    {
        lcb_destroy(mInstance);
        mInstance = NULL;
    }
}

bool CBDataSource::Upsert(QString key, QString document)
{
    //TODO: Exercise 8a

    return false;
}

bool CBDataSource::Upsert(QString key, QJsonObject document)
{
    //TODO: Excercise 8b

    return false;
}

bool CBDataSource::Delete(QString key)
{
    //TODO: Exercise 10

    return false;
}


bool CBDataSource::IncrCounter(QString name, int delta, int initial)
{
    QByteArray ba_key = name.toUtf8();
    const char *c_key = ba_key.data();

    lcb_arithmetic_cmd_t cmd;
    memset(&cmd, 0, sizeof cmd);
    const lcb_arithmetic_cmd_t *cmdlist = &cmd;
    cmd.v.v0.key = c_key;
    cmd.v.v0.nkey = strlen(c_key);
    cmd.v.v0.delta = delta; // Increment by
    cmd.v.v0.initial = initial; // Set to this if it does not exist
    cmd.v.v0.create = 1; // Create item if it does not exist
    lcb_error_t err = lcb_arithmetic(mInstance, NULL, 1, &cmdlist);
    return (err == LCB_SUCCESS);
}

QString CBDataSource::Get(QString key)
{
    //TODO: Exercise 9a

    return "";
}

QJsonObject CBDataSource::GetJsonObject(QString key)
{
    QJsonObject result;

    //TODO: Exercise 9b

    return result;
}

CouchbaseValueMap CBDataSource::MultiGet(QStringList keys)
{
    MultiGetResult mg;

    //TODO: Exercise 11

    return mg.items;
}

QueryResult viewCallbackResults;

static void viewCallback(lcb_t instance, int ign, const lcb_RESPVIEWQUERY *rv)
{
    if (rv->rflags & LCB_RESP_F_FINAL)
    {
        QByteArray baMeta = QByteArray((const char*)rv->value, rv->nvalue);
        QString meta = QString(baMeta);

        qDebug() << "*** META FROM VIEWS ***";
        qDebug() << meta;

        QJsonObject json = QJsonDocument::fromJson(meta.toUtf8()).object();
        viewCallbackResults.total = json["total_rows"].toInt();
        return;
    }

    QueryResultEntry entry;

    QByteArray baDocId = QByteArray((const char*)rv->docid, rv->ndocid);
    QString docId = QString(baDocId);
    entry.key = docId;
    QByteArray baValue = QByteArray((const char*)rv->value, rv->nvalue);
    entry.value = QString(baValue);
    viewCallbackResults.items.append(entry);
}

QueryResult CBDataSource::QueryView(QString designDocName, QString viewName, int limit, int skip)
{
    //TODO: Exercise 12

    return QueryResult();
}

N1qlResult n1qlCallbackResults;

static void n1qlCallback(lcb_t instance, int cbtype, const lcb_RESPN1QL *resp)
{
    if (! (resp->rflags & LCB_RESP_F_FINAL))
    {
        QByteArray baRow = QByteArray((const char*)resp->row, resp->nrow);
        QString qsRow = QString(baRow);
        QJsonObject jsonRow = QJsonDocument::fromJson(qsRow.toUtf8()).object();
        n1qlCallbackResults.items.append(jsonRow);
    }
}

N1qlResult CBDataSource::QueryN1ql(QString query)
{
    //TODO: Exercise 13

    return N1qlResult();

}

