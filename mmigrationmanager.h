#pragma once

#include <QSqlDatabase>
#include <QVersionNumber>

#include <functional>
#include <type_traits>

#include "connectionproviders/mconnectionproviderbase.h"
#include "mabstractmigrationmanager.h"

template <typename T, typename = void>
struct has_instance_method : std::false_type {};

template <typename T>
struct has_instance_method<T, 
           std::void_t<decltype(T::instance()) >> : std::true_type {};

namespace MDatabase {
class Migration;

template<class ConnectionProvider, typename Valid = std::enable_if_t<
            std::is_base_of<ConnectionProviderBase, ConnectionProvider>::value
            && has_instance_method<ConnectionProvider>::value>>
class MigrationManager : public AbstractMigrationManager
{
public:
    MigrationManager(const QString &connectionName = 
                              QLatin1String(QSqlDatabase::defaultConnection));

    void loadVersion();

    bool needsUpdate();
    bool update();

    void setupDatabase() override;

    const ConnectionProvider& provider() const {
        return ConnectionProvider::instance();
    }

protected:
    const QString m_dbConnectionName;

    QVersionNumber m_dbVersion;

    QVersionNumber getVersionNumber() const;
    bool updateDb();

    template<typename It>
    It findMigrationNumber(It begin, It end, const QVersionNumber &number);

    template<typename It>
    bool applyMigrations(It begin, It end, 
            std::function<bool(const Migration &)> const &handler, bool forward);
};
}

// Migrations Manager implementation
#include <QSqlQuery>
#include <QSqlError>
#include <QLoggingCategory>
#include <QtConcurrent/QtConcurrent>

#include "mdbhelpers.h"
#include "mmigration.h"
#include "mmigrationsdata.h"

Q_DECLARE_LOGGING_CATEGORY(mdatabase)
namespace MDatabase {
// Migration manager implementation
template<class ConnectionProvider, typename Valid>
MigrationManager<ConnectionProvider, Valid>::MigrationManager(const QString &connectionName)
    : m_dbConnectionName(connectionName)
{}

template<class ConnectionProvider, typename Valid>
void MigrationManager<ConnectionProvider, Valid>::loadVersion()
{
    m_dbVersion = getVersionNumber();
}

template<class ConnectionProvider, typename Valid>
bool MigrationManager<ConnectionProvider, Valid>::needsUpdate()
{
    return (m_dbVersion != Migrations::latestDbVersion());
}

template<class ConnectionProvider, typename Valid>
bool MigrationManager<ConnectionProvider, Valid>::update()
{
    if (!needsUpdate()) {
        qCInfo(mdatabase) << "Database up to date.";
        return true;
    }

    return updateDb();
}

template<class ConnectionProvider, typename Valid>
void MigrationManager<ConnectionProvider, Valid>::setupDatabase()
{
    qDebug() << "Setting up DB" << m_dbConnectionName << m_dbVersion;
    Q_ASSERT_X(!m_setupDone, Q_FUNC_INFO, "Trying to setup database twice");
    if (!m_setupDone) {
        qDebug() << "YYYYYY";
        loadVersion();
        qDebug() << "ZZZZZZ";
        if (needsUpdate()) {
            emit databaseUpdateStarted();
            qDebug() << "DB update started HERE" << m_dbConnectionName << m_dbVersion;

            //update();

            m_migrationRunner = QtConcurrent::run(
                std::bind(&MigrationManager::update, this));
            m_migrationProgress.setFuture(m_migrationRunner);
        } else {
            emit databaseReady();
        }

        m_setupDone = true;
    }
}

template<class ConnectionProvider, typename Valid>
QVersionNumber MigrationManager<ConnectionProvider, Valid>::getVersionNumber() const
{
    static const QLatin1String VersionQuery = 
            QLatin1String("SELECT `version` from `Migrations` ORDER BY `id` DESC LIMIT 1");

    qDebug() << "XXX" << m_dbConnectionName;
    auto query = QSqlQuery(provider().databaseConnection(m_dbConnectionName));
    query.prepare(VersionQuery);
    qDebug() << "AAAAA" << m_dbConnectionName;
    MDatabase::Helpers::execQuery(query);
    if (!query.first()) {
        qCCritical(mdatabase) << "No version in migration table.";
        return {};
    }

    return QVersionNumber::fromString(query.value(0).toString());
}

template<class ConnectionProvider, typename Valid>
bool MigrationManager<ConnectionProvider, Valid>::updateDb()
{
    auto db = provider().databaseConnection(m_dbConnectionName);
    auto dbName = db.connectionName();

    if (m_dbVersion > Migrations::latestDbVersion()) {
        // backward
        return applyMigrations(Migrations::migrations().rbegin(), Migrations::migrations().rend(),
            std::bind(Migration::RunBackward, std::placeholders::_1, db), false);
    }

    // forward
    return applyMigrations(Migrations::migrations().begin(), Migrations::migrations().end(),
            std::bind(Migration::RunForward, std::placeholders::_1, db), true);
}

template<class ConnectionProvider, typename Valid>
template<typename It>
bool MigrationManager<ConnectionProvider, Valid>::applyMigrations(
                It begin, It end, 
                std::function<bool(const Migration &)> const &handler, bool forward)
{
    auto start = begin;
    if (!m_dbVersion.isNull()) {
        start = findMigrationNumber(begin, end, m_dbVersion);
        if (start == end) {
            qCCritical(mdatabase) << "Cannot update database - version missing.";
            return false;
        }
        start += (forward ? 1 : 0);
    }

    auto finish = findMigrationNumber(begin, end, Migrations::latestDbVersion());
    if (finish == end) {
        qCCritical(mdatabase) << "Cannot update database - latest version missing.";
        return false;
    }
    finish += (forward ? 1 : 0);

    return std::all_of(start, finish, handler);
}

template<class ConnectionProvider, typename Valid>
template<typename It>
It MigrationManager<ConnectionProvider, Valid>::findMigrationNumber(
                                It begin, It end, const QVersionNumber &number)
{
    auto item = std::find_if(begin, end, 
            [&number](const Migration &migration) {
                return (migration.number() == number); 
            });
    if (item == end) {
        qCCritical(mdatabase) << "Version not found in migrations! Version:" << number;
    }

    return item;
}
}
