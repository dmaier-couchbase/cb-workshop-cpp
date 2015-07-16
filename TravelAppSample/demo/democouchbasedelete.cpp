#include "democouchbasedelete.h"
#include <qdebug.h>

void DemoCouchbaseDelete::test()
{
    CBDataSource& ds = CBDataSourceFactory::Instance();

   QJsonObject json;
   json.insert("msg", "to delete");
   ds.Upsert("test::delete", json);

   qDebug() << "Created document test::delete";

   bool success = ds.Delete("test::delete");

   if (success)
   {
       qDebug() << "Successfully deleted the document!";
   }
   else
   {
       qDebug() << "Could not delete the document";
   }

}
