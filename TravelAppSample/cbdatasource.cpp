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
    QByteArray tmp = connectionString.toUtf8();
    struct lcb_create_st cropts;
    cropts.version = 3;
    cropts.v.v3.connstr = tmp.data();
    lcb_error_t err;
    err = lcb_create(&mInstance, &cropts);

    if (err != LCB_SUCCESS)
    {
        qDebug() << QString("Couldn't create instance: %1").arg(QString::fromUtf8(lcb_strerror(mInstance, err)));
        exit(1); // fatal
    }
    else
    {
        qDebug() << "Successfully created Couchbase instance.";
    }

    err = lcb_connect(mInstance);
    if (err != LCB_SUCCESS)
    {
        qDebug() << QString("Connect failed: %1").arg(QString::fromUtf8(lcb_strerror(mInstance, err)));
        exit(1); // fatal
    }
    else
    {
        qDebug() << "Successfully created Couchbase instance.";
    }

    lcb_wait(mInstance);
    if ((err = lcb_get_bootstrap_status(mInstance)) != LCB_SUCCESS)
    {
        qDebug() << QString("Couldn't bootstrap: %1").arg(QString::fromUtf8(lcb_strerror(mInstance, err)));
        exit(1);
    }
    else
    {
        qDebug() << "Sucessfully bootstrapped!";
    }

    lcb_set_store_callback(mInstance, storage_callback);
    lcb_set_get_callback(mInstance, get_callback);

    mIsConnected = true;
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
    lcb_error_t err;
    lcb_store_cmd_t scmd;
    memset(&scmd, 0, sizeof scmd);
    scmd.version = 0;
    const lcb_store_cmd_t *scmdlist = &scmd;
    QByteArray ba_doc = document.toUtf8();
    const char *c_doc = ba_doc.data();
    QByteArray ba_key = key.toUtf8();
    const char *c_key = ba_key.data();
    scmd.v.v0.key = c_key;

    //Optionally use the CAS value
    //scmd.v.v0.cas
    //scmd.v.v0.datatype = LCB_VALUE_F_JSON;
    scmd.v.v0.nkey = strlen(c_key);
    scmd.v.v0.bytes = c_doc;
    scmd.v.v0.nbytes = strlen(c_doc);
    scmd.v.v0.operation = LCB_SET;

    err = lcb_store(mInstance, NULL, 1, &scmdlist);
    if (err != LCB_SUCCESS)
    {
        qDebug() << QString("Couldn't schedule storage operation: %1").arg(QString::fromUtf8(lcb_strerror(mInstance, err)));
        return false;
    }
    else
    {
        lcb_wait(mInstance);
        return true;
    }
}

bool CBDataSource::Upsert(QString key, QJsonObject document)
{
    QJsonDocument doc(document);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    return Upsert(key, strJson);
}

bool CBDataSource::Delete(QString key)
{
    lcb_error_t err;
    QByteArray ba_key = key.toUtf8();
    const char *c_key = ba_key.data();

    lcb_set_remove_callback(mInstance, on_removed);
    lcb_remove_cmd_t cmd;
    memset(&cmd, 0, sizeof cmd);
    const lcb_remove_cmd_t *cmdlist = &cmd;
    cmd.v.v0.key = c_key;
    cmd.v.v0.nkey = strlen(c_key);
    err = lcb_remove(mInstance, NULL, 1, &cmdlist);

    if (err != LCB_SUCCESS)
    {
        qDebug() << QString("Couldn't schedule remove operation: %1").arg(QString::fromUtf8(lcb_strerror(mInstance, err)));
        return false;
    }
    else
    {
        lcb_wait(mInstance);
        return removedSuccessfully;
    }
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
    lcb_error_t err;
    lcb_get_cmd_t gcmd;
    memset(&gcmd, 0, sizeof gcmd);
    const lcb_get_cmd_t *gcmdlist = &gcmd;
    QByteArray ba_key = key.toUtf8();
    const char *c_key = ba_key.data();
    gcmd.v.v0.key = c_key;
    gcmd.v.v0.nkey = strlen(c_key);
    err = lcb_get(mInstance, NULL, 1, &gcmdlist);
    if (err != LCB_SUCCESS)
    {
        qDebug() << QString("Couldn't schedule get operation: %1").arg(QString::fromUtf8(lcb_strerror(mInstance, err)));
        return "";
    }
    lcb_wait(mInstance);
    return result;
}

QJsonObject CBDataSource::GetJsonObject(QString key)
{
    QJsonObject result;
    QJsonObject meta;
    QString resultDocumentString = Get(key);

    meta["cas"] = (long long)cas;
    result["meta"] = meta;
    result["doc"] = QJsonDocument::fromJson(resultDocumentString.toUtf8()).object();

    return result;
}

CouchbaseValueMap CBDataSource::MultiGet(QStringList keys)
{
    multiGet = true;
    MultiGetResult mg;
    for (QStringList::iterator it = keys.begin(); it != keys.end(); ++it)
    {
        QByteArray ba_key = it->toUtf8();
        const char *c_key = ba_key.data();

        lcb_get_cmd_t cmd;
        memset(&cmd, 0, sizeof cmd);
        const lcb_get_cmd_t *cmdlist = &cmd;
        cmd.v.v0.key = c_key;
        cmd.v.v0.nkey = strlen(c_key);
        lcb_get(mInstance, &mg, 1, &cmdlist);
    }
    lcb_wait(mInstance);
    multiGet = false;
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
    lcb_CMDVIEWQUERY vq;

    QString optQstring = QString("limit=%1&skip=%2&stale=false").arg(QString::number(limit), QString::number(skip));
    QByteArray optBA = optQstring.toUtf8();
    char* optChar = optBA.data();
    vq.optstr = optChar;
    vq.noptstr = strlen(optChar);

    viewCallbackResults.items.clear();
    viewCallbackResults.limit = limit;
    viewCallbackResults.skip = skip;

    QByteArray  baDesignDoc =  designDocName.toUtf8();
    char* pDesignDoc = baDesignDoc.data();

    QByteArray baView = viewName.toUtf8();
    char* pView = baView.data();

    lcb_view_query_initcmd(&vq, pDesignDoc, pView, NULL, viewCallback);
    lcb_error_t rc = lcb_view_query(mInstance, NULL, &vq);

    if (rc != LCB_SUCCESS)
    {
        qDebug() << QString("Could not execute view query: %1").arg(QString::fromUtf8(lcb_strerror(mInstance, rc)));
        return QueryResult();
    }
    else
    {
        lcb_wait(mInstance);
        return viewCallbackResults;
    }
}

N1clResult n1clCallbackResults;

static void niclCallback(lcb_t instance, int cbtype, const lcb_RESPN1QL *resp)
{
    if (! (resp->rflags & LCB_RESP_F_FINAL))
    {
        QByteArray baRow = QByteArray((const char*)resp->row, resp->nrow);
        QString qsRow = QString(baRow);
        QJsonObject jsonRow = QJsonDocument::fromJson(qsRow.toUtf8()).object();
        n1clCallbackResults.items.append(jsonRow);
    }
}

N1clResult CBDataSource::QueryN1cl(QString query)
{
    QByteArray baQuery = query.toUtf8();
    char* pQuery = baQuery.data();

    n1clCallbackResults.items.clear();

    lcb_CMDN1QL cmd;
    memset(&cmd, 0, sizeof cmd);
    lcb_N1QLPARAMS *nparams = lcb_n1p_new();
    lcb_n1p_setstmtz(nparams, pQuery);

    lcb_n1p_mkcmd(nparams, &cmd);

    cmd.callback = niclCallback;
    lcb_error_t rc = lcb_n1ql_query(mInstance, NULL, &cmd);
    if (rc != LCB_SUCCESS)
    {
        qDebug() << QString("N1cl Query could not be executed: %1").arg(QString::fromUtf8(lcb_strerror(mInstance, rc)));
        return N1clResult();
    }
    lcb_n1p_free(nparams);
    lcb_wait(mInstance);
    return n1clCallbackResults;
}

