#include "couchbasevalue.h"

#include <QJsonDocument>

CouchbaseValue::CouchbaseValue(const lcb_get_resp_t *resp, lcb_error_t rc)
{
    mError = rc;
    if (!success())
    {
        return;
    }
    QByteArray ba = QByteArray((const char*)resp->v.v0.bytes, resp->v.v0.nbytes);
    mData = QString(ba);
    mCas = resp->v.v0.cas;
}

bool CouchbaseValue::success() const
{
    return mError == LCB_SUCCESS;
}

QString CouchbaseValue::asString()
{
    return mData;
}

QJsonObject CouchbaseValue::asJson()
{
    return QJsonDocument::fromJson(mData.toLatin1()).object();
}

uint64_t CouchbaseValue::cas()
{
    return mCas;
}

lcb_error_t CouchbaseValue::error()
{
    return mError;
}



