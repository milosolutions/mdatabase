#pragma once

#include "mconnectionprovidersqlite.h"

namespace MDatabase {
class ConnectionProviderSQLiteSee : public ConnectionProviderSQLite
{
public:
    static MDatabase::ConnectionProviderSQLiteSee *instance();

    void setPassword(const QString& password);

    virtual QSqlDatabase databaseConnection(const QString &connectionName =
                        QSqlDatabase::defaultConnection) const override;

    virtual void setupConnectionData(
            const QString &databasePath,
            const QString &connectionName = QLatin1String(
                QSqlDatabase::defaultConnection)) override;

protected:
    ConnectionProviderSQLiteSee();

private:
    QString m_password;
};
}
