#include "dbconnectionproviderbase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QSqlQuery>
#include <QSqlError>

db::ConnectionProviderBase::ConnectionProviderBase(const QString &databaseType)
    : m_databaseType(databaseType)
{}

db::ConnectionProviderBase::~ConnectionProviderBase()
{}

bool db::ConnectionProviderBase::hasDatabaseConnection(const QString &connectionName) const
{
    return QSqlDatabase::contains(extendedConnectionName(connectionName));
}

QSqlDatabase db::ConnectionProviderBase::databaseConnection(const QString &connectionName) const
{
    auto db = QSqlDatabase{};
    if (hasDatabaseConnection(connectionName)) {
        db = QSqlDatabase::database(extendedConnectionName(connectionName));
    } else {
        Q_ASSERT_X(QSqlDatabase::contains(baseConnectionName(connectionName)),
                   __PRETTY_FUNCTION__, "Database used before setup!");

        db = QSqlDatabase::cloneDatabase(baseConnectionName(connectionName), extendedConnectionName(connectionName));
    }

    if (!db.isOpen() && !db.open()) {
        qCritical() << "Cannot open database connection. Cannot proceed.";
    }
    return db;
}

QSqlDatabase db::ConnectionProviderBase::createDatabaseConnection(const QString &connectionName) const
{
    return QSqlDatabase::addDatabase(m_databaseType, baseConnectionName(connectionName));
}

QString db::ConnectionProviderBase::baseConnectionName(const QString &connectionName) const
{
    return QString("%1_%2").arg(m_databaseType).arg(connectionName);
}

QString db::ConnectionProviderBase::extendedConnectionName(const QString &connectionName) const
{
    auto baseConnectionName = this->baseConnectionName(connectionName);
    if (QCoreApplication::instance()->thread() == QThread::currentThread()) {
        return baseConnectionName;
    }

    auto currentThreadIdx = QString("0x%1").arg((quintptr)(QThread::currentThread()),
                                                QT_POINTER_SIZE * 2, 16, QChar('0'));
    return QString("%1_%2").arg(baseConnectionName).arg(currentThreadIdx);
}