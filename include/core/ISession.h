#ifndef ISESSION_H
#define ISESSION_H

#include <memory>
#include <string>
#include <vector>
#include <map>
#include "mapping/IEntity.h"
#include "database/IDatabaseConnection.h"
#include "ITransaction.h"
#include "query/IQuery.h"
#include "TransactionDefinition.h"

class ISession {
public:
    virtual ~ISession() = default;

    // 엔티티 저장
    virtual void save(std::shared_ptr<IEntity> entity) = 0;
    // 엔티티 업데이트
    virtual void update(std::shared_ptr<IEntity> entity) = 0;
    // 엔티티 삭제
    virtual void remove(std::shared_ptr<IEntity> entity) = 0;
    // 엔티티 조회
    virtual std::shared_ptr<IEntity> find(const std::string& entityName, int id) = 0;
    // 쿼리 생성
    virtual std::shared_ptr<IQuery> createQuery(const std::string& queryString) = 0;
    // 트랜잭션 관리
    virtual std::shared_ptr<ITransaction> beginTransaction(const TransactionDefinition& definition = TransactionDefinition()) = 0;
    // 세션 닫기
    virtual void close() = 0;
};

#endif // ISESSION_H