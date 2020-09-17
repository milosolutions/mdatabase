#pragma once

#include <QVersionNumber>
#include <QVector>
#include <array>
namespace MDatabase {
    class Migration;
    class Migrations {
    public:
        static const QVersionNumber& latestDbVersion()
        {
            Q_ASSERT_X(!m_version.isNull(), Q_FUNC_INFO,
                       "Migrations were no set properly, check manual");
            return m_version;
        }

        static const QVector<Migration>& migrations()
        {
            Q_ASSERT_X(!m_migrations.empty(), Q_FUNC_INFO,
                       "Migrations were no set properly, check manual");
            return m_migrations;
        }

        static void init(const QVersionNumber& version, const QVector<Migration>& migrations)
        {
            m_version = version;
            m_migrations = migrations;
        }

    private:
        static QVersionNumber m_version;
        static QVector<Migration> m_migrations;
    };
}


#define INIT_MIGRATIONS { \
    extern void dbmigrationsinit(); \
    dbmigrationsinit(); }

#define CURRENT_MIGRATION_VERSION(vmajor, vminor, vpatch) \
    const QVersionNumber LATEST_DB_VERSION = { vmajor, vminor, vpatch };

#define START_MIGRATIONS const QVector<MDatabase::Migration> DB_MIGRATIONS = {

#define END_MIGRATIONS    };                                    \
    extern void dbmigrationsinit()                              \
    { Migrations::init(LATEST_DB_VERSION, DB_MIGRATIONS); }
