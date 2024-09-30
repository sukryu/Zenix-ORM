#ifndef IQUERY_H
#define IQUERY_H

#include <string>
#include <vector>
#include <memory>
#include "IEntity.h"
#include <map>

class IQuery {
public:
    virtual ~IQuery() = default;

    // 파라미터 설정
    virtual void setParameter(const std::string& name, const std::string& value) = 0;

    // 쿼리 실행
    virtual std::vector<std::shared_ptr<IEntity>> list() = 0;
    virtual std::shared_ptr<IEntity> uniqueResult() = 0;

    // 결과를 엔티티가 아닌 맵으로 반환 (필요한 경우)
    virtual std::vector<std::map<std::string, std::string>> listMap() = 0;
};

#endif // IQUERY_H