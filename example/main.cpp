#include <QCoreApplication>

#define AppVersion "0.0.1"
#define CompanyName "Milo Solutions"
#define AppName "MigrationsMCDB"

#include <QSqlQuery>
#include <QStandardPaths>

#include "mmigrationmanager.h"
#include "mdbhelpers.h"
#include <QLoggingCategory>

#include <connectionproviders/mconnectionprovidersqlite.h>
#include <connectionproviders/mconnectionprovidersqlitecipher.h>

Q_DECLARE_LOGGING_CATEGORY(mdatabase)

namespace mdb = MDatabase;

void printUsers(const QSqlDatabase& connection)
{
    auto query = QSqlQuery(connection);
    query.prepare("SELECT * FROM `User`;");
    mdb::Helpers::execQuery(query);

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
    mdb::ConnectionProviderSQLite::instance().setupConnectionData(
                QStandardPaths::writableLocation(
                    QStandardPaths::AppDataLocation) + "/local.db");


    // SQlite migrations manager 
    using SqliteMigrations = mdb::MigrationManager<mdb::ConnectionProviderSQLite>;
    SqliteMigrations dbManager;
    QObject::connect(&dbManager, &SqliteMigrations::databaseUpdateStarted,
                     &app, []{ qInfo() << "Database update started!"; });
    QObject::connect(&dbManager, &SqliteMigrations::databaseUpdateError,
                     &app, []{ qInfo() << "Database update error!"; });
    QObject::connect(&dbManager, &SqliteMigrations::databaseReady,
                     &app, [&]{
                        qCInfo(mdatabase) << "Database ready!";
                        printUsers(dbManager.provider().databaseConnection());
                     });
    dbManager.setupDatabase();



    // **** SQLITE 3 Cipher ****

    qCDebug(mdatabase) << "==================================";
    qCDebug(mdatabase) << "SQlite3Cipher example:";

    auto& cipher= mdb::ConnectionProviderSQLiteCipher::instance();
    if (cipher.checkPluginAvailable()) {
        cipher.setPassword("secret");
        cipher.setupConnectionData(QStandardPaths::writableLocation
                               (QStandardPaths::AppDataLocation)+ "/localCipher.db");
        using SqliteCipherMigrations = mdb::MigrationManager<mdb::ConnectionProviderSQLiteCipher>;
        SqliteCipherMigrations cipherManager;
        QObject::connect(&cipherManager, &SqliteCipherMigrations::databaseUpdateStarted,
                         &app, []{ qInfo() << "Ciphered database update started!"; });
        QObject::connect(&cipherManager, &SqliteCipherMigrations::databaseUpdateError,
                         &app, []{ qInfo() << "Ciphered database update error!"; });
        QObject::connect(&cipherManager, &SqliteCipherMigrations::databaseReady,
                         &app, [&]{
                            qCInfo(mdatabase) << "Ciphered database ready!";
                            printUsers(cipherManager.provider().databaseConnection());
                         });
        cipherManager.setupDatabase();
    } else {
        qCCritical(mdatabase) << "SQLite3 Cipher Plugin was not found!!!"
                               << "Check mmigraions/scripts/build_sqlitecipherplugin.sh "
                                  "script for details";

    }

    return app.exec();
}
