#include "mconnectionprovidersqlitecipher.h"

#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(migrations)

MDatabase::ConnectionProviderSQLiteCipher &MDatabase::ConnectionProviderSQLiteCipher::instance()
{
    static ConnectionProviderSQLiteCipher cp;
    return cp;
}

MDatabase::ConnectionProviderSQLiteCipher::ConnectionProviderSQLiteCipher() :
    ConnectionProviderSQLite("SQLITECIPHER")
{

}

void MDatabase::ConnectionProviderSQLiteCipher::setPassword(const QString &password)
{
    mPassword = password;
}

QSqlDatabase MDatabase::ConnectionProviderSQLiteCipher::databaseConnection(const QString &connectionName) const
{
    auto db = ConnectionProviderBase::databaseConnection(connectionName);
    db.setPassword(mPassword);
    return db;
}

void MDatabase::ConnectionProviderSQLiteCipher::setupConnectionData(
                    const QString &databasePath, const QString &connectionName)
{
    if (!databaseExist(databasePath)) {
        createDatabase(databasePath);
    }

    auto db = createDatabaseConnection(connectionName);
    db.setDatabaseName(databasePath);
    db.setPassword(mPassword);
}

bool MDatabase::ConnectionProviderSQLiteCipher::checkPluginAvailable() const
{
    return !QSqlDatabase::drivers().filter("SQLITECIPHER").empty();
}
