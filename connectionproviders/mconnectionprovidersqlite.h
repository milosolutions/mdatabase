#pragma once

#include "mconnectionproviderbase.h"

namespace MDatabase {
class ConnectionProviderSQLite : public ConnectionProviderBase
{
public:
    static MDatabase::ConnectionProviderSQLite &instance();

    virtual void setupConnectionData(
            const QString &databasePath,
            const QString &connectionName = QLatin1String(QSqlDatabase::defaultConnection));

    static bool databaseExists(const QString &databasePath);
    static bool createDatabase(const QString &databasePath);

protected:
    ConnectionProviderSQLite();
    ConnectionProviderSQLite(const QString& type);

private:
    static const QString m_pluginName;
};
}
