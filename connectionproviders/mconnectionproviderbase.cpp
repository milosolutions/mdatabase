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

bool MDatabase::ConnectionProviderBase::checkPluginAvailable() const
{
    return QSqlDatabase::drivers().contains(m_databaseType);
}

bool MDatabase::ConnectionProviderBase::hasDatabaseConnection(
                                        const QString &connectionName) const
{
    return QSqlDatabase::contains(connectionName);
}

QSqlDatabase MDatabase::ConnectionProviderBase::databaseConnection(
                                        const QString &connectionName) const
{
    QSqlDatabase db;
    const QString extendedName = extendedConnectionName(connectionName);
    if (hasDatabaseConnection(extendedName)) {
        db = QSqlDatabase::database(extendedName);
    } else {
        if (QSqlDatabase::contains(baseConnectionName(connectionName))) {
            qCWarning(mdatabase) << "Database used before setup!"
                                 << connectionName;
        }

        db = QSqlDatabase::cloneDatabase(baseConnectionName(connectionName),
                                         extendedName);
    }

    if (db.isOpen() == false && db.open() == false) {
        qCCritical(mdatabase) << "Cannot open database connection. Cannot proceed."
                              << db.databaseName() << db.connectionName()
                              << db.driverName();
        return db;
    }

    qCDebug(mdatabase) << "Opened database" << db.databaseName()
                       << db.connectionName() << db.driverName();

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

    const auto currentThreadIdx = QString("0x%1").arg(
                reinterpret_cast<quintptr>(QThread::currentThread()),
                QT_POINTER_SIZE * 2, 16, QChar('0'));
    return QString("%1_%2").arg(baseConnectionName, currentThreadIdx);
}
