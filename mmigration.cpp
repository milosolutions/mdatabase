#include "mmigration.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>

#include "mdbhelpers.h"

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(mdatabase)

#include "mmigrationsdata.h"

QVector<MDatabase::Migration> MDatabase::Migrations::m_migrations;
QVersionNumber MDatabase::Migrations::m_version;

MDatabase::Migration::Migration(const QVersionNumber &number,
                         const std::function<bool (QSqlDatabase &)> &forward,
                         const std::function<bool (QSqlDatabase &)> &backward)
    : m_number(number), m_forward(forward), m_backward(backward)
{}

bool MDatabase::Migration::RunForward(const Migration &m, QSqlDatabase &db)
{
    m.runCommon(db);
    auto result = m.m_forward(db);

    if (result) {
        static const QLatin1String addMigrationEntryQuery("INSERT INTO Migrations"
                           "(`timestamp`, `version`) VALUES (:timestamp, :number)");
        auto query = QSqlQuery(db);
        query.prepare(addMigrationEntryQuery);
        query.bindValue(":timestamp", QDateTime::currentDateTime().toSecsSinceEpoch());
        query.bindValue(":number", m.m_number.toString());

        Helpers::execQuery(query);
        result &= !Helpers::hasError(query);
    }

    return result;
}

bool MDatabase::Migration::RunBackward(const Migration &m, QSqlDatabase &db)
{
    m.runCommon(db);
    auto result = m.m_backward(db);

    if (result) {
        static const QLatin1String removeMigrationEntryQuery = 
            QLatin1String("DELETE FROM `Migrations` WHERE `version` IS :number");
        auto query = QSqlQuery(db);
        query.prepare(removeMigrationEntryQuery);
        query.bindValue(":number", m.m_number.toString());

        MDatabase::Helpers::execQuery(query);
        result &= !MDatabase::Helpers::hasError(query);
    }

    return result;
}

QVersionNumber MDatabase::Migration::number() const
{
    return m_number;
}

void MDatabase::Migration::runCommon(QSqlDatabase &db) const
{
    Q_ASSERT_X(db.isOpen(), Q_FUNC_INFO,
               "Database object should always be opened for migration!");
}


/*!
 * \class MigrationBuilder
 * This class simplifies creation of migrations in list.
 * Is uses fluent builder patter (modified because there is no need
 * for inheritance (Migration is final)
 */

/**
 * This static method returns very first migration
 * which creates Migrations table with version and a timestamp
 */
MDatabase::Migration MDatabase::MigrationBuilder::migration001()
{
    return MigrationBuilder::builder()
            .setVersion("0.0.1")
            .addForwardQuery(
                "CREATE TABLE `Migrations` "
                "(`id` INTEGER primary key AUTOINCREMENT,"
                " `timestamp` INTEGER NOT NULL, `version` TEXT NOT NULL)")
            .addBackwardQuery("DROP TABLE `Migrations`")
            .build();
}

/**
 *  Creates a builder object to start building
 */
MDatabase::MigrationBuilder MDatabase::MigrationBuilder::builder()
{
    return MDatabase::MigrationBuilder();
}

/**
 *  Finalizes building and returns final Migration object
 */
MDatabase::Migration MDatabase::MigrationBuilder::build()
{
    Q_ASSERT_X(!m_version.isNull(), Q_FUNC_INFO,
               "Version was not set!");
    Q_ASSERT_X(!m_forward.empty(), Q_FUNC_INFO,
               "Forward queries collection is empty!");
    Q_ASSERT_X(!m_fackward.empty(), Q_FUNC_INFO,
               "Backward queries collection is empty!");

    auto forward = [f = std::move(m_forward)](const QSqlDatabase &database) {
                        return Helpers::runQueries(database, f);
                   };
    auto backward = [b = std::move(m_fackward)](const QSqlDatabase &database) {
                        return Helpers::runQueries(database, b);
                   };
    return MDatabase::Migration(m_version, forward, backward);
}

/**
 * Sets migration version (major, minor, micro numbers)
 */
MDatabase::MigrationBuilder&& MDatabase::MigrationBuilder::setVersion(const QVersionNumber &version)
{
    m_version = version;
    return std::move(*this);
}

/**
 * Sets migration version. Version should have format acceptable by QVersionNumber,
 * like this : "1.1.0" (major, minor, micro numbers)
 */
MDatabase::MigrationBuilder &&MDatabase::MigrationBuilder::setVersion(const QString &version)
{
    m_version = QVersionNumber::fromString(version);
    return std::move(*this);
}

/**
 * Adds query that is used in forward migration. Every invokation of this method
 * will add new query after last one.
 */
MDatabase::MigrationBuilder &&MDatabase::MigrationBuilder::addForwardQuery(const QLatin1String &query)
{
    m_forward.push_back(query);
    return std::move(*this);
}

/**
 * Adds query that is used in forward migration. Every invokation of this method
 * will add new query after last one.
 */
MDatabase::MigrationBuilder &&MDatabase::MigrationBuilder::addForwardQuery(const char* query)
{
    return addForwardQuery(QLatin1String(query));
}

/**
 * Adds query that is used in backward migration. Every invokation of this method
 * will add new query after last one.
 */
MDatabase::MigrationBuilder &&MDatabase::MigrationBuilder::addBackwardQuery(const QLatin1String &query)
{
    m_fackward.push_back(query);
    return std::move(*this);
}

/**
 * Adds query that is used in backward migration. Every invokation of this method
 * will add new query after last one.
 */
MDatabase::MigrationBuilder &&MDatabase::MigrationBuilder::addBackwardQuery(const char* query)
{
    return addBackwardQuery(QLatin1String(query));
}

/**
 * Sets all queries that are used in forward migration. It is assumed that no forward queries
 * were added before this method invokation
 */
MDatabase::MigrationBuilder &&MDatabase::MigrationBuilder::setForwardQueries(const MDatabase::Helpers::Queries &queries)
{
    Q_ASSERT_X(m_forward.empty(), Q_FUNC_INFO, "mForward collection is not empty!");
    m_forward = queries;
    return std::move(*this);
}

/**
 * Sets all queries that are used in backward migration. It is assumed that no backward queries
 * were added before this method invokation
 */
MDatabase::MigrationBuilder &&MDatabase::MigrationBuilder::setBackwardQueries(const MDatabase::Helpers::Queries &queries)
{
    Q_ASSERT_X(m_fackward.empty(), Q_FUNC_INFO, "mForward collection is not empty!");
    m_fackward = queries;
    return std::move(*this);
}
