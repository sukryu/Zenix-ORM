#ifndef SESSION_H
#define SESSION_H

#include "ISession.h"
#include "database/IDatabaseConnection.h"
#include "utils/logger/Logger.h"
#include "mapping/EntityMapper.h"
#include "Transaction.h"
#include "query/Query.h"
#include "query/QueryBuilder.h"

class Session : public ISession {
public:
    explicit Session(std::shared_ptr<IDatabaseConnection> connection);
    virtual ~Session();

    void save(std::shared_ptr<IEntity> entity) override;
    void update(std::shared_ptr<IEntity> entity) override;
    void remove(std::shared_ptr<IEntity> entity) override;
    std::shared_ptr<IEntity> find(const std::string& entityName, int id) override;
    std::shared_ptr<IQuery> createQuery(const std::string& queryString) override;
    std::shared_ptr<ITransaction> beginTransaction() override;
    std::shared_ptr<IQueryBuilder> createQueryBuilder();
    void close() override;

private:
    std::shared_ptr<IDatabaseConnection> connection;
    Logger& logger;
    bool isTransactionActive;
    std::unordered_map<std::string, std::shared_ptr<IEntity>> entityCache; // 1차 캐시
};

#endif // SESSION_H