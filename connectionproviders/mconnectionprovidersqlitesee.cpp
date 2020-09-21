#include "mconnectionprovidersqlitesee.h"

#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QSqlQuery>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(mdatabase)

MDatabase::ConnectionProviderSQLiteSee &MDatabase::ConnectionProviderSQLiteSee::instance()
{
    static ConnectionProviderSQLiteSee cp;
    return cp;
}

void MDatabase::ConnectionProviderSQLiteSee::setPassword(const QString &password)
{
    m_password = password;
}

QSqlDatabase MDatabase::ConnectionProviderSQLiteSee::databaseConnection(
        const QString &connectionName) const
{
    auto db = ConnectionProviderBase::databaseConnection(connectionName);
    QSqlQuery key(QString("PRAGMA key='%1'").arg(m_password), db);
    return db;
}

void MDatabase::ConnectionProviderSQLiteSee::setupConnectionData(const QString &databasePath, const QString &connectionName)
{
    if (!databaseExists(databasePath)) {
        createDatabase(databasePath);
    }

    qDebug() << "Creating DB SEE" << m_databaseType << connectionName;

    auto db = createDatabaseConnection(connectionName);
    db.setDatabaseName(databasePath);
}

MDatabase::ConnectionProviderSQLiteSee::ConnectionProviderSQLiteSee()
    : ConnectionProviderSQLite("QSQLITESEE")
{}
