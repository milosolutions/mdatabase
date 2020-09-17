#include "mconnectionproviderbase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QSqlQuery>
#include <QSqlError>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(mdatabase)

MDatabase::ConnectionProviderBase::ConnectionProviderBase(const QString &databaseType)
    : m_databaseType(databaseType)
{}


bool MDatabase::ConnectionProviderBase::hasDatabaseConnection(
                                        const QString &connectionName) const
{
    return QSqlDatabase::contains(extendedConnectionName(connectionName));
}

QSqlDatabase MDatabase::ConnectionProviderBase::databaseConnection(
                                        const QString &connectionName) const
{
    auto db = QSqlDatabase{};
    if (hasDatabaseConnection(connectionName)) {
        db = QSqlDatabase::database(extendedConnectionName(connectionName));
    } else {
        Q_ASSERT_X(QSqlDatabase::contains(baseConnectionName(connectionName)),
                   Q_FUNC_INFO, "Database used before setup!");

        db = QSqlDatabase::cloneDatabase(baseConnectionName(connectionName), 
                                    extendedConnectionName(connectionName));
    }

    if (!db.isOpen() && !db.open()) {
        qCCritical(mdatabase) << "Cannot open database connection. Cannot proceed.";
    }
    return db;
}

QSqlDatabase MDatabase::ConnectionProviderBase::createDatabaseConnection(
                                        const QString &connectionName) const
{
    return QSqlDatabase::addDatabase(m_databaseType,
                        baseConnectionName(connectionName));
}

QString MDatabase::ConnectionProviderBase::baseConnectionName(
                            const QString &connectionName) const
{
    return QString("%1_%2").arg(m_databaseType, connectionName);
}

QString MDatabase::ConnectionProviderBase::extendedConnectionName(
                                        const QString &connectionName) const
{
    auto baseConnectionName = this->baseConnectionName(connectionName);
    if (QCoreApplication::instance()->thread() == QThread::currentThread()) {
        return baseConnectionName;
    }

    auto currentThreadIdx = QString("0x%1").arg(
                        reinterpret_cast<quintptr>(QThread::currentThread()),
                                    QT_POINTER_SIZE * 2, 16, QChar('0'));
    return QString("%1_%2").arg(baseConnectionName, currentThreadIdx);
}
