#ifndef DBCONNECTIONPROVIDERSQLITESEE_H
#define DBCONNECTIONPROVIDERSQLITESEE_H

#include "mconnectionproviderbase.h"

namespace MDatabase {
class ConnectionProviderSQLiteSee : public ConnectionProviderBase
{
public:
    static MDatabase::ConnectionProviderSQLiteSee &instance();

    virtual void setupConnectionData(const QString &databasePath,
                const QString &connectionName = QLatin1String(QSqlDatabase::defaultConnection));

    static bool databaseExist(const QString &databasePath);
    static bool createDatabase(const QString &databasePath);

protected:
    ConnectionProviderSQLiteSee();
    ConnectionProviderSQLiteSee(const QString& type);
};
}

#endif // DBCONNECTIONPROVIDERSQLITESEE_H
