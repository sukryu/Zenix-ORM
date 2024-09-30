#ifndef QUERY_H
#define QUERY_H

#include "IQuery.h"
#include "database/DatabaseConnectionFactory.h"
#include "../mapping/EntityMapper.h"
#include "utils/logger/Logger.h"
#include "ORMException/ORMException.h"
#include <unordered_map>
#include <sqlite3.h>

class Query : public IQuery {
public:
    Query(std::shared_ptr<IDatabaseConnection> connection, const std::string& queryString);
    virtual ~Query();

    void setParameter(const std::string& name, const std::string& value) override;

    std::vector<std::shared_ptr<IEntity>> list() override;
    std::shared_ptr<IEntity> uniqueResult() override;

    std::vector<std::map<std::string, std::string>> listMap() override;

private:
    std::shared_ptr<IDatabaseConnection> connection;
    std::string queryString;
    std::unordered_map<std::string, std::string> parameters;
    Logger& logger;

    // SQLite3 전용: 준비된 문 (statement)
    sqlite3_stmt* stmt;

    // 쿼리를 준비하고 파라미터를 바인딩하는 함수
    void prepareAndBind();
};

#endif // QUERY_H