#include "mconnectionprovidersqlitecipher.h"

#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(mdatabase)

const QString MDatabase::ConnectionProviderSQLiteCipher::m_pluginName = "SQLITECIPHER";

MDatabase::ConnectionProviderSQLiteCipher &MDatabase::ConnectionProviderSQLiteCipher::instance()
{
    static ConnectionProviderSQLiteCipher cp;
    return cp;
}

MDatabase::ConnectionProviderSQLiteCipher::ConnectionProviderSQLiteCipher() :
    ConnectionProviderSQLite(m_pluginName)
{

}

void MDatabase::ConnectionProviderSQLiteCipher::setPassword(const QString &password)
{
    m_password = password;
}

QSqlDatabase MDatabase::ConnectionProviderSQLiteCipher::databaseConnection(const QString &connectionName) const
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

bool MDatabase::ConnectionProviderSQLiteCipher::checkPluginAvailable() const
{
    return !QSqlDatabase::drivers().filter(m_pluginName).empty();
}
