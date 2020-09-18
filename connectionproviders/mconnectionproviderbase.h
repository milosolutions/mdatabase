#pragma once

#include <QSqlDatabase>
#include <QString>
#include <QHash>

namespace MDatabase {
class ConnectionProviderBase
{
public:
    ConnectionProviderBase(const QString &databaseType);
    virtual ~ConnectionProviderBase() = default;

    bool checkPluginAvailable() const;
    virtual bool hasDatabaseConnection(const QString &connectionName =
                        QSqlDatabase::defaultConnection) const;
    virtual QSqlDatabase databaseConnection(const QString &connectionName =
                        QSqlDatabase::defaultConnection) const;

protected:
    QString m_databaseType;

    virtual QSqlDatabase createDatabaseConnection(const QString &connectionName) const;

    QString baseConnectionName(const QString &connectionName) const;
    QString extendedConnectionName(const QString &connectionName) const;
};
}
