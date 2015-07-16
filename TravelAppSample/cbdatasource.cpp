#include "cbdatasource.h"
#include "multiget.h"

#include <libcouchbase/couchbase.h>
#include <libcouchbase/views.h>
#include <stdio.h>

#include <QStringList>
#include <QJsonObject>
#include <QJsonDocument>

static void
storage_callback(lcb_t instance, const void *cookie, lcb_storage_t op, lcb_error_t err,
const lcb_store_resp_t *resp)
{
    if (err != LCB_SUCCESS) {
        printf("Couldnt store item to cluster : %s\n", lcb_strerror(instance, err));
    }
    printf("Stored %.*s\n", (int)resp->v.v0.nkey, resp->v.v0.key);
}

static QString result;
static bool multiGet = false;

static void
get_callback(lcb_t instance, const void *cookie, lcb_error_t err, const lcb_get_resp_t *resp)
{
    if (multiGet)
    {
        MultiGet *mg = const_cast<MultiGet*>(reinterpret_cast<const MultiGet*>(cookie));
        mg->handleResponse(resp, err);
    }
    else
    {
        printf("Retrieved key %.*s\n", (int)resp->v.v0.nkey, resp->v.v0.key);
        printf("Value is %.*s\n", (int)resp->v.v0.nbytes, resp->v.v0.bytes);

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
        fprintf(stderr, "Failed to remove item : %s\n", lcb_strerror(instance, err));
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
    QByteArray tmp = connectionString.toLatin1();
    struct lcb_create_st cropts;
    cropts.version = 3;
    cropts.v.v3.connstr = tmp.data();
    lcb_error_t err;
    err = lcb_create(&mInstance, &cropts);

    if (err != LCB_SUCCESS) {
        printf("Couldn't create instance!\n");

        //TODO: Exception Handling instead
        exit(1);
    }
   else
    {
        //TODO: Use Qt Logging here instead
        printf("Successfully created Couchbase instance.\n");
    }

    lcb_connect(mInstance);
    lcb_wait(mInstance);
    if ((err = lcb_get_bootstrap_status(mInstance)) != LCB_SUCCESS) {
        printf("Couldn't bootstrap!\n");
        exit(1);
    }
    else
    {
        printf("Sucessfully bootstrapped!\n");
    }

    lcb_set_store_callback(mInstance, storage_callback);
    lcb_set_get_callback(mInstance, get_callback);

    mIsConnected = true;
}

CBDataSource::~CBDataSource()
{
    if (mIsConnected)
    {
        lcb_destroy(mInstance);
        mInstance = NULL;
    }
}

void CBDataSource::Upsert(QString key, QString document)
{
    lcb_error_t err;
    lcb_store_cmd_t scmd;
    scmd.version = 0;
    const lcb_store_cmd_t *scmdlist = &scmd;
    QByteArray ba_doc = document.toLatin1();
    const char *c_doc = ba_doc.data();
    QByteArray ba_key = key.toLatin1();
    const char *c_key = ba_key.data();
    scmd.v.v0.key = c_key;

    //TODO: Optionally use the CAS value
    //scmd.v.v0.cas
    //scmd.v.v0.datatype = LCB_VALUE_F_JSON;
    scmd.v.v0.nkey = strlen(c_key);
    scmd.v.v0.bytes = c_doc;
    scmd.v.v0.nbytes = strlen(c_doc);
    scmd.v.v0.operation = LCB_SET;

    err = lcb_store(mInstance, NULL, 1, &scmdlist);
    if (err != LCB_SUCCESS) {
        printf("Couldn't schedule storage operation!\n");
        exit(1);
    }
    lcb_wait(mInstance); //storage_callback is invoked here
}

void CBDataSource::Upsert(QString key, QJsonObject document)
{
    QJsonDocument doc(document);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    Upsert(key, strJson);
}

bool CBDataSource::Delete(QString key)
{
    lcb_error_t err;
    QByteArray ba_key = key.toLatin1();
    const char *c_key = ba_key.data();

    lcb_set_remove_callback(mInstance, on_removed);
    lcb_remove_cmd_t cmd;
    const lcb_remove_cmd_t *cmdlist = &cmd;
    cmd.v.v0.key = c_key;
    cmd.v.v0.nkey = strlen(c_key);
    err = lcb_remove(mInstance, NULL, 1, &cmdlist);
    if (err != LCB_SUCCESS)
    {
        printf("Couldn't schedule remove operation: %s\n", lcb_strerror(mInstance, err));
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
    QByteArray ba_key = name.toLatin1();
    const char *c_key = ba_key.data();

    lcb_arithmetic_cmd_t cmd;
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
    const lcb_get_cmd_t *gcmdlist = &gcmd;
    QByteArray ba_key = key.toLatin1();
    const char *c_key = ba_key.data();
    gcmd.v.v0.key = c_key;
    gcmd.v.v0.nkey = strlen(c_key);
    err = lcb_get(mInstance, NULL, 1, &gcmdlist);
    if (err != LCB_SUCCESS) {
        printf("Couldn't schedule get operation!\n");
        exit(1);
    }
    lcb_wait(mInstance); // get_callback is invoked here
    return result;
}

QJsonObject CBDataSource::GetJsonObject(QString key)
{
    //TODO: Use a JsonObject which has the following structure:
    // { meta : { cas : 'xyz' }, doc : { ... } }


    QString result = Get(key);
    return QJsonDocument::fromJson(result.toLatin1()).object();
}

CouchbaseValueMap CBDataSource::MultiGet(QStringList keys)
{
    multiGet = true;
    MultiGet mg;
    for (QStringList::iterator it = keys.begin(); it != keys.end(); ++it)
    {
        QByteArray ba_key = it->toLatin1();
        const char *c_key = ba_key.data();

        lcb_get_cmd_t cmd;
        memset(&cmd, 0, sizeof cmd);
        const lcb_get_cmd_t *cmdlist = &cmd;
        cmd.v.v0.key = c_key;
        cmd.v.v0.nkey = strlen(c_key);
        lcb_get(mInstance, &mg, 1, &cmdlist);
    }
    lcb_wait(instance);
    multiGet = false;
    return mg.items;
}

QueryResult viewCallbackResults;

static void viewCallback(lcb_t instance, int ign, const lcb_RESPVIEWQUERY *rv)
{
    if (rv->rflags & LCB_RESP_F_FINAL) {
        printf("*** META FROM VIEWS ***\n");
        fprintf(stderr, "%.*s\n", (int)rv->nvalue, rv->value);
        QByteArray baMeta = QByteArray((const char*)rv->value, rv->nvalue);
        QString meta = QString(baMeta);
        QJsonObject json = QJsonDocument::fromJson(meta.toLatin1()).object();
        viewCallbackResults.total = json["total_rows"].toInt();
        return;
    }

    QByteArray baDocId = QByteArray((const char*)rv->docid, rv->ndocid);
    QString docId = QString(baDocId);
    QByteArray baKey = QByteArray((const char*)rv->key, rv->nkey);
    QString key = QString(baKey);
    viewCallbackResults.keys.append(docId);

    printf("Got row callback from LCB: RC=0x%X, DOCID=%.*s. KEY=%.*s\n",
        rv->rc,
        (int)rv->ndocid, rv->docid,
        (int)rv->nkey, rv->key);

    if (rv->docresp) {
        printf("   Document for response. RC=0x%X. CAS=0x%lx\n",
            rv->docresp->rc, rv->docresp->cas);
    }
}

QueryResult CBDataSource::QueryView(int limit, int skip)
{
    lcb_CMDVIEWQUERY vq;

    QString optQstring = QString("limit=%1&skip=%2").arg(QString::number(limit), QString::number(skip));
    QByteArray optBA = optQstring.toLatin1();
    char* optChar = optBA.data();
    vq.optstr = optChar;
    vq.noptstr = strlen(optChar);

    viewCallbackResults.keys.clear();
    viewCallbackResults.limit = limit;
    viewCallbackResults.skip = skip;

    lcb_view_query_initcmd(&vq, "beer", "by_name", NULL, viewCallback);
    lcb_error_t rc = lcb_view_query(mInstance, NULL, &vq);
    if (rc != LCB_SUCCESS) {
        // Handle error
    }
    lcb_wait(mInstance);
    return viewCallbackResults;
}
