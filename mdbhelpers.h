#pragma once

#include <QString>

class QSqlDatabase;
class QSqlQuery;

namespace MDatabase {
class Helpers
{
public:
    using Queries = QVector<QLatin1String>;
    static bool hasError(const QSqlQuery &query);

    static bool execQuery(QSqlQuery &query);
    static bool execQuery(const QSqlDatabase &database, const QLatin1String &queryStr);

    static bool runQueries(const QSqlDatabase &database, const Queries &queries);
};

}
