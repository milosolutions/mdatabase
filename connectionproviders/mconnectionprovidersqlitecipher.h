#ifndef DBCONNECTIONPROVIDERSQLITECIPHER_H
#define DBCONNECTIONPROVIDERSQLITECIPHER_H

#include "mconnectionprovidersqlite.h"

namespace MDatabase {
class ConnectionProviderSQLiteCipher : public ConnectionProviderSQLite
{
public:
    static MDatabase::ConnectionProviderSQLiteCipher &instance();

    ConnectionProviderSQLiteCipher();
    bool checkPluginAvailable() const;
    void setPassword(const QString& password);

    virtual QSqlDatabase databaseConnection(const QString &connectionName =
                        QLatin1String(QSqlDatabase::defaultConnection)) const;

    virtual void setupConnectionData(const QString &databasePath,
                const QString &connectionName = QLatin1String(QSqlDatabase::defaultConnection));
private:
    QString m_password;
};
}
#endif
