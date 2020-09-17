#pragma once

#include "mconnectionproviderbase.h"

namespace MDatabase {
class ConnectionProviderSQLite : public ConnectionProviderBase
{
public:
    static MDatabase::ConnectionProviderSQLite &instance();

    virtual void setupConnectionData(
            const QString &databasePath,
            const QString &connectionName = QSqlDatabase::defaultConnection);

    static bool databaseExists(const QString &databasePath);
    static bool createDatabase(const QString &databasePath);

protected:
    ConnectionProviderSQLite();
    ConnectionProviderSQLite(const QString& type);
};
}
