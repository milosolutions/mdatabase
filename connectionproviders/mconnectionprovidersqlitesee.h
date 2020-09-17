#pragma once

#include "mconnectionprovidersqlite.h"

namespace MDatabase {
class ConnectionProviderSQLiteSee : public ConnectionProviderSQLite
{
public:
    static MDatabase::ConnectionProviderSQLiteSee &instance();    

    bool checkPluginAvailable() const;
    void setPassword(const QString& password);

    virtual QSqlDatabase databaseConnection(const QString &connectionName =
                        QLatin1String(QSqlDatabase::defaultConnection)) const override;

protected:
    ConnectionProviderSQLiteSee();

private:
    static const QString m_pluginName;
    QString m_password;
};
}
