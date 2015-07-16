#ifndef COUCHBASEVALUE_H
#define COUCHBASEVALUE_H

#include <QString>
#include <QJsonObject>
#include <libcouchbase/couchbase.h>

class CouchbaseValue
{
public:
    CouchbaseValue(const lcb_get_resp_t *resp, lcb_error_t rc);
    bool success() const;
    QString asString();
    QJsonObject asJson();
    uint64_t cas();
    lcb_error_t error();

private:
    QString mData;
    uint64_t mCas;
    lcb_error_t mError;
};

#endif // COUCHBASEVALUE_H

