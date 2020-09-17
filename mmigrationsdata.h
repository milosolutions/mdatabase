#ifndef DBMIGRATIONSDATA_H
#define DBMIGRATIONSDATA_H

#include <QVersionNumber>
#include <QVector>
#include <array>
namespace MDatabase {
    class Migration;
    class Migrations {
    public:
        static const QVersionNumber& latestDbVersion()
        {
            Q_ASSERT_X(!sVersion.isNull(), Q_FUNC_INFO,
                       "Migrations were no set properly, check manual");
            return sVersion;
        }

        static const QVector<Migration>& migrations()
        {
            Q_ASSERT_X(!sMigrations.empty(), Q_FUNC_INFO,
                       "Migrations were no set properly, check manual");
            return sMigrations;
        }

        static void init(const QVersionNumber& version, const QVector<Migration>& migrations)
        {
            sVersion = version;
            sMigrations = migrations;
        }

    private:
        static QVersionNumber sVersion;
        static QVector<Migration> sMigrations;
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

#endif // DBMIGRATIONSDATA_H
