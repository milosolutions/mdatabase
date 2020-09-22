#include "mconnectionprovidersqlitecipher.h"

#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(mdatabase)

MDatabase::ConnectionProviderSQLiteCipher *MDatabase::ConnectionProviderSQLiteCipher::instance()
{
    static ConnectionProviderSQLiteCipher cp;
    return &cp;
}

void MDatabase::ConnectionProviderSQLiteCipher::setPassword(const QString &password)
{
    m_password = password;
}

QSqlDatabase MDatabase::ConnectionProviderSQLiteCipher::databaseConnection(
        const QString &connectionName) const
{
    auto db = ConnectionProviderBase::databaseConnection(connectionName);
    db.setPassword(m_password);
    return db;
}

void MDatabase::ConnectionProviderSQLiteCipher::setupConnectionData(
                    const QString &databasePath, const QString &connectionName)
{
    if (!databaseExists(databasePath)) {
        createDatabase(databasePath);
    }

    auto db = createDatabaseConnection(connectionName);
    db.setDatabaseName(databasePath);
    db.setPassword(m_password);
}

MDatabase::ConnectionProviderSQLiteCipher::ConnectionProviderSQLiteCipher() :
    ConnectionProviderSQLite("SQLITECIPHER")
{
}
