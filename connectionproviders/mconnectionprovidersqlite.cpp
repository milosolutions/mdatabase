#include "mconnectionprovidersqlite.h"

#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(mdatabase)

const QString MDatabase::ConnectionProviderSQLite::m_pluginName = "QSQLITE";

MDatabase::ConnectionProviderSQLite &MDatabase::ConnectionProviderSQLite::instance()
{
    static ConnectionProviderSQLite cp;
    return cp;
}

void MDatabase::ConnectionProviderSQLite::setupConnectionData(
                    const QString &databasePath, const QString &connectionName)
{
    if (!databaseExists(databasePath)) {
        createDatabase(databasePath);
    }

    auto db = createDatabaseConnection(connectionName);
    db.setDatabaseName(databasePath);
}

bool MDatabase::ConnectionProviderSQLite::databaseExists(const QString &databasePath)
{
    return QFile::exists(databasePath);
}

bool MDatabase::ConnectionProviderSQLite::createDatabase(const QString &databasePath)
{
    if (!QFileInfo(databasePath).absoluteDir().mkpath(".")) {
        qCCritical(mdatabase) << "Cannot create a directory for database: " << databasePath;
        return false;
    }
    return true;
}

MDatabase::ConnectionProviderSQLite::ConnectionProviderSQLite()
    : ConnectionProviderBase(m_pluginName)
{}

MDatabase::ConnectionProviderSQLite::ConnectionProviderSQLite(const QString &type)
    : ConnectionProviderBase(type)
{}
