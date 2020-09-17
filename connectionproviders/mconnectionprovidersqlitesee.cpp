#include "mconnectionprovidersqlitesee.h"

#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QSqlQuery>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(mdatabase)

const QString MDatabase::ConnectionProviderSQLiteSee::m_pluginName = "QSQLITESEE";

MDatabase::ConnectionProviderSQLiteSee &MDatabase::ConnectionProviderSQLiteSee::instance()
{
    static ConnectionProviderSQLiteSee cp;
    return cp;
}

bool MDatabase::ConnectionProviderSQLiteSee::checkPluginAvailable() const
{
    return !QSqlDatabase::drivers().filter(m_pluginName).empty();
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

MDatabase::ConnectionProviderSQLiteSee::ConnectionProviderSQLiteSee()
    : ConnectionProviderSQLite(m_pluginName)
{}
