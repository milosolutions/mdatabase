#include <QCoreApplication>
#include <QTimer>

#define AppVersion "0.0.1"
#define CompanyName "Milo Solutions"
#define AppName "MDatabase"

#include <QSqlQuery>
#include <QStandardPaths>
#include <QLoggingCategory>

#include "mmigrationmanager.h"
#include "mdbhelpers.h"

#include <connectionproviders/mconnectionprovidersqlite.h>
#include <connectionproviders/mconnectionprovidersqlitecipher.h>
#include <connectionproviders/mconnectionprovidersqlitesee.h>

Q_DECLARE_LOGGING_CATEGORY(mdatabase)

void printUsers(const QSqlDatabase& connection)
{
    auto query = QSqlQuery(connection);
    query.prepare("SELECT * FROM `User`;");
    MDatabase::Helpers::execQuery(query);

    qCDebug(mdatabase) << "USERS:";
    while (query.next()) {
        auto userName = query.value(1).toString();
        qCDebug(mdatabase) << userName;
    }
}

int main(int argc, char *argv[])
{
    INIT_MIGRATIONS

    QCoreApplication app(argc, argv);
    app.setApplicationVersion(AppVersion);
    app.setOrganizationName(CompanyName);
    app.setApplicationName(AppName);

    // **** SQLITE 3 ****
    qCDebug(mdatabase) << "==================================";
    qCDebug(mdatabase) << "SQLite3 example:";
    // setting up SQLite db path
    MDatabase::ConnectionProviderSQLite::instance()->setupConnectionData(
                QStandardPaths::writableLocation(
                    QStandardPaths::AppDataLocation) + "/local.db");

    // SQlite migrations manager
    using SqliteMigrations = MDatabase::MigrationManager<MDatabase::ConnectionProviderSQLite>;
    SqliteMigrations dbManager;
    QObject::connect(&dbManager, &SqliteMigrations::databaseUpdateStarted,
                     &app, []{ qInfo() << "Database update started!"; });
    QObject::connect(&dbManager, &SqliteMigrations::databaseUpdateError,
                     &app, []{ qInfo() << "Database update error!"; });
    QObject::connect(&dbManager, &SqliteMigrations::databaseReady,
                     &app, [&]{
        qCInfo(mdatabase) << "Database ready!";
        printUsers(dbManager.provider()->databaseConnection());
    });
    dbManager.setupDatabase();

    // **** SQLITE 3 Cipher ****

    qCDebug(mdatabase) << "==================================";
    qCDebug(mdatabase) << "SQlite3Cipher example:";

    auto cipher = MDatabase::ConnectionProviderSQLiteCipher::instance();
    using SqliteCipherMigrations = MDatabase::MigrationManager<MDatabase::ConnectionProviderSQLiteCipher>;
    SqliteCipherMigrations cipherManager;

    if (cipher->checkPluginAvailable()) {
        cipher->setPassword("secret");
        cipher->setupConnectionData(QStandardPaths::writableLocation
                                    (QStandardPaths::AppDataLocation) + "/localCipher.db");
        QObject::connect(&cipherManager, &SqliteCipherMigrations::databaseUpdateStarted,
                         &app, []{ qInfo() << "Ciphered database update started!"; });
        QObject::connect(&cipherManager, &SqliteCipherMigrations::databaseUpdateError,
                         &app, []{ qInfo() << "Ciphered database update error!"; });
        QObject::connect(&cipherManager, &SqliteCipherMigrations::databaseReady,
                         &app, [&]{
            qCInfo(mdatabase) << "Ciphered database ready!";
            printUsers(cipherManager.provider()->databaseConnection());
        });
        cipherManager.setupDatabase();
    } else {
        qCCritical(mdatabase) << "SQLite3 Cipher Plugin was not found!!!"
                              << "Check mdatabase/sqlitecipher "
                                 "script for details";
    }

    // **** SQLiteSee ****

    qCDebug(mdatabase) << "==================================";
    qCDebug(mdatabase) << "SQliteSee example:";

    auto see = MDatabase::ConnectionProviderSQLiteSee::instance();
    using SqliteSeeMigrations = MDatabase::MigrationManager<MDatabase::ConnectionProviderSQLiteSee>;
    SqliteSeeMigrations seeManager;

    if (see->checkPluginAvailable()) {
        see->setPassword("secret1234");
        see->setupConnectionData(QStandardPaths::writableLocation
                                 (QStandardPaths::AppDataLocation) + "/localSee.db");
        QObject::connect(&seeManager, &SqliteSeeMigrations::databaseUpdateStarted,
                         &app, []{ qInfo() << "SEE database update started!"; });
        QObject::connect(&seeManager, &SqliteSeeMigrations::databaseUpdateError,
                         &app, []{ qInfo() << "SEE database update error!"; });
        QObject::connect(&seeManager, &SqliteSeeMigrations::databaseReady,
                         &app, [&]{
            qCInfo(mdatabase) << "SEE database ready!";
            printUsers(seeManager.provider()->databaseConnection());
        });
        seeManager.setupDatabase();
    } else {
        qCCritical(mdatabase) << "SQLiteSee Plugin was not found!!!"
                              << "Check mdatabase/sqlitesee "
                                 "script for details";
    }

    // Auto-quit the app
    QTimer::singleShot(1000, &app, &QCoreApplication::quit);

    return app.exec();
}
