#include "mabstractmigrationmanager.h"

#include <QFuture>
#include <QLoggingCategory>
#include <QStandardPaths>

Q_DECLARE_LOGGING_CATEGORY(mdatabase)

namespace MDatabase {

AbstractMigrationManager::AbstractMigrationManager(QObject *parent)
    : QObject(parent)
{
    connect(&m_migrationProgress, &QFutureWatcher<bool>::finished,
            this, [this]() {
        if (!m_migrationProgress.result()) {
            emit databaseUpdateError();
        }

        emit databaseReady();
    });
}

}

