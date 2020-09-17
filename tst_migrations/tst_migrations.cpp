/*******************************************************************************
Copyright (C) 2017 Milo Solutions
Contact: https://www.milosolutions.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

#include <QtTest>
#include <QCoreApplication>
#include "mmigrationmanager.h"

#include "connectionproviders/mconnectionprovidersqlite.h"
#include "mmigration.h"
#include "mmigrationsdata.h"

#include <cstdio>

namespace MDatabase {
    const QVersionNumber LATEST_DB_VERSION = { 0, 0, 3 };
    const QVector<MDatabase::Migration> DB_MIGRATIONS;
}


class TestMDatabase : public QObject
{
   Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testMigrationBuilder();

private:
    QString mDbPath;
};

void TestMDatabase::initTestCase()
{
    QCoreApplication::setApplicationName("MDatabase Test");
    QCoreApplication::setOrganizationName("Milo");

    mDbPath = QStandardPaths::writableLocation(
                    QStandardPaths::AppDataLocation) + "/test.db";
    MDatabase::ConnectionProviderSQLite::instance().setupConnectionData(mDbPath);

    // SQlite migrations manager
    using SqliteMigrations = MDatabase::MigrationManager<MDatabase::ConnectionProviderSQLite>;
    SqliteMigrations dbManager;
}

void TestMDatabase::cleanupTestCase()
{
    QFile::remove(mDbPath);
}

void TestMDatabase::testMigrationBuilder()
{
    MDatabase::Migration m = MDatabase::MigrationBuilder::builder()
        .setVersion("0.0.3")
        .addForwardQuery("CREATE TABLE `User`  (`id` INTEGER primary key UNIQUE)")
        .addBackwardQuery("DROP TABLE `User`")
        .build();

    
    QCOMPARE(m.number(), QVersionNumber(0, 0, 3));
}


QTEST_MAIN(TestMDatabase)

#include "tst_migrations.moc"
