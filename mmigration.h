#pragma once

#include <QVersionNumber>
#include <functional>
#include <QVector>

#include "mdbhelpers.h"

class QSqlDatabase;

namespace MDatabase {
class Migration final {
public:
    Migration(const QVersionNumber &mNumber,
              const std::function<bool(QSqlDatabase &)> &mForward,
              const std::function<bool(QSqlDatabase &)> &mBackward);

    static bool RunForward(const Migration &m, QSqlDatabase &database);
    static bool RunBackward(const Migration &m, QSqlDatabase &database);

    QVersionNumber number() const;

private:
    QVersionNumber m_number;
    std::function<bool(QSqlDatabase &)> m_forward;
    std::function<bool(QSqlDatabase &)> m_backward;

    void runCommon(QSqlDatabase &database) const;
};


class MigrationBuilder
{
public:
    static Migration migration001();
    static MigrationBuilder builder();
    Migration build();
    MigrationBuilder&& setVersion(const QVersionNumber& version);
    MigrationBuilder&& setVersion(const QString& version);
    MigrationBuilder&& addForwardQuery(const QLatin1String& query);
    MigrationBuilder&& addForwardQuery(const char* query);
    MigrationBuilder&& addBackwardQuery(const QLatin1String& query);
    MigrationBuilder&& addBackwardQuery(const char* query);
    MigrationBuilder&& setForwardQueries(const Helpers::Queries& queries);
    MigrationBuilder&& setBackwardQueries(const Helpers::Queries& queries);

private:
    MigrationBuilder() = default;
    Helpers::Queries m_forward;
    Helpers::Queries m_fackward;
    QVersionNumber m_version;
};

}
