#include "mconnectionprovidersqlitesee.h"

#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(migrations)

MDatabase::ConnectionProviderSQLiteSee &MDatabase::ConnectionProviderSQLiteSee::instance()
{
    static ConnectionProviderSQLiteSee cp;
    return cp;
}

void MDatabase::ConnectionProviderSQLiteSee::setupConnectionData(
                    const QString &databasePath, const QString &connectionName)
{
    if (!databaseExist(databasePath)) {
        createDatabase(databasePath);
    }

    auto db = createDatabaseConnection(connectionName);
    db.setDatabaseName(databasePath);
}

bool MDatabase::ConnectionProviderSQLiteSee::databaseExist(const QString &databasePath)
{
    return QFile::exists(databasePath);
}

bool MDatabase::ConnectionProviderSQLiteSee::createDatabase(const QString &databasePath)
{
    if (!QFileInfo(databasePath).absoluteDir().mkpath(".")) {
        qCCritical(migrations) << "Cannot create a directory for database: " << databasePath;
        return false;
    }
    return true;
}

MDatabase::ConnectionProviderSQLiteSee::ConnectionProviderSQLiteSee()
    : ConnectionProviderBase("QSQLITESEE")
{}

MDatabase::ConnectionProviderSQLiteSee::ConnectionProviderSQLiteSee(const QString &type)
    : ConnectionProviderBase(type)
{}
