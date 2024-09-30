#ifndef IQUERY_BUILDER_H
#define IQUERY_BUILDER_H

#include <string>
#include <memory>
#include "IQuery.h"

class IQueryBuilder {
public:
    virtual ~IQueryBuilder() = default;

    virtual IQueryBuilder& select(const std::string& fields) = 0;
    virtual IQueryBuilder& from(const std::string& table, const std::string& alias = "") = 0;
    virtual IQueryBuilder& where(const std::string& condition) = 0;
    virtual IQueryBuilder& orderBy(const std::string& field, const std::string& order = "ASC") = 0;
    virtual IQueryBuilder& limit(int limit) = 0;
    virtual IQueryBuilder& offset(int offset) = 0;

    virtual std::shared_ptr<IQuery> getQuery() = 0;
};

#endif // IQUERY_BUILDER_H