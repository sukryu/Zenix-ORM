#ifndef QUERY_BUILDER_H
#define QUERY_BUILDER_H

#include "IQueryBuilder.h"
#include "../database/IDatabaseConnection.h"
#include "../utils/logger/Logger.h"

class QueryBuilder : public IQueryBuilder {
public:
    QueryBuilder(std::shared_ptr<IDatabaseConnection> connection);
    virtual ~QueryBuilder();

    IQueryBuilder& select(const std::string& fields) override;
    IQueryBuilder& from(const std::string& table, const std::string& alias = "") override;
    IQueryBuilder& where(const std::string& condition) override;
    IQueryBuilder& orderBy(const std::string& field, const std::string& order = "ASC") override;
    IQueryBuilder& limit(int limit) override;
    IQueryBuilder& offset(int offset) override;

    std::shared_ptr<IQuery> getQuery() override;

private:
    std::shared_ptr<IDatabaseConnection> connection;
    Logger& logger;

    std::string selectClause;
    std::string fromClause;
    std::string whereClause;
    std::string orderByClause;
    std::string limitClause;
    std::string offsetClause;
};

#endif // QUERY_BUILDER_H