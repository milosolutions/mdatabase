#pragma once

#include "mconnectionprovidersqlite.h"

namespace MDatabase {
class ConnectionProviderSQLiteCipher : public ConnectionProviderSQLite
{
public:
    static MDatabase::ConnectionProviderSQLiteCipher &instance();

    void setPassword(const QString& password);

    virtual QSqlDatabase databaseConnection(const QString &connectionName =
                        QSqlDatabase::defaultConnection) const override;

    virtual void setupConnectionData(
            const QString &databasePath,
            const QString &connectionName = QLatin1String(
                QSqlDatabase::defaultConnection)) override;

protected:
    ConnectionProviderSQLiteCipher();

private:
    QString m_password;
};
}
