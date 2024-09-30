#include "query/QueryBuilder.h"
#include "query/Query.h"

QueryBuilder::QueryBuilder(std::shared_ptr<IDatabaseConnection> connection)
    : connection(connection), logger(Logger::getInstance()) {
    logger.debug("QueryBuilder created.");
}

QueryBuilder::~QueryBuilder() {
    logger.debug("QueryBuilder destroyed.");
}

IQueryBuilder& QueryBuilder::select(const std::string& fields) {
    selectClause = "SELECT " + fields;
    return *this;
}

IQueryBuilder& QueryBuilder::from(const std::string& table, const std::string& alias) {
    fromClause = "FROM " + table;
    if (!alias.empty()) {
        fromClause += " " + alias;
    }
    return *this;
}

IQueryBuilder& QueryBuilder::where(const std::string& condition) {
    if (whereClause.empty()) {
        whereClause = "WHERE " + condition;
    } else {
        whereClause += " AND " + condition;
    }
    return *this;
}

IQueryBuilder& QueryBuilder::orderBy(const std::string& field, const std::string& order) {
    orderByClause = "ORDER BY " + field + " " + order;
    return *this;
}

IQueryBuilder& QueryBuilder::limit(int limit) {
    limitClause = "LIMIT " + std::to_string(limit);
    return *this;
}

IQueryBuilder& QueryBuilder::offset(int offset) {
    offsetClause = "OFFSET " + std::to_string(offset);
    return *this;
}

std::shared_ptr<IQuery> QueryBuilder::getQuery() {
    std::string queryString = selectClause + " " + fromClause;
    if (!whereClause.empty()) {
        queryString += " " + whereClause;
    }
    if (!orderByClause.empty()) {
        queryString += " " + orderByClause;
    }
    if (!limitClause.empty()) {
        queryString += " " + limitClause;
    }
    if (!offsetClause.empty()) {
        queryString += " " + offsetClause;
    }

    logger.debug("Generated query string: " + queryString);

    return std::make_shared<Query>(connection, queryString);
}