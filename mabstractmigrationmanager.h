#pragma once

#include <QFutureWatcher>

namespace MDatabase {

class AbstractMigrationManager : public QObject
{
    Q_OBJECT
public:
    explicit AbstractMigrationManager(QObject* parent = nullptr);

    virtual void setupDatabase() = 0;

signals:
    void databaseReady() const;
    void databaseUpdateStarted() const;
    void databaseUpdateError() const;

protected:
    bool m_setupDone = false;

    QFuture<bool> m_migrationRunner;
    QFutureWatcher<bool> m_migrationProgress;

    void onMigrationFinished(bool result);
};

}
