#ifndef MULTIGET_H
#define MULTIGET_H

#include "couchbasevalue.h"

#include <QMap>
#include <QString>

#include <libcouchbase/couchbase.h>

class MultiGet
{
public:
    QMap<QString, CouchbaseValue> items;
    void handleResponse(const lcb_get_resp_t *resp, lcb_error_t err)
    {
        QByteArray ba = QByteArray((const char*)resp->v.v0.bytes, resp->v.v0.nbytes);
        QString key(ba);
        items.insert(key, CouchbaseValue(resp, err));
    }
};

#endif // MULTIGET_H

