#ifndef IDATABASE_CONNECTION_H
#define IDATABASE_CONNECTION_H

#include <string>
#include <vector>
#include <map>
#include "../utils/logger/Logger.h"
#include "../ORMException/ORMException.h"

class IDatabaseConnection {
public:
	virtual ~IDatabaseConnection() = default;

	// 데이터베이스 연결 및 해제
	virtual void connect() = 0;
	virtual void disconnect() = 0;

	// 쿼리 실행
	virtual std::vector<std::map<std::string, std::string>> executeQuery(const std::string& query) = 0;
	virtual int executeUpdate(const std::string& query) = 0;

	// 트랜잭션 관리
	virtual void beginTransaction() = 0;
	virtual void commit() = 0;
	virtual void rollback() = 0;

	// Native handle 반환 (SQLite의 경우 sqlite3*)
	virtual sqlite3* getNativeHandle() = 0;

	// 쿼리에서 테이블 이름을 추출하는 함수
    virtual std::string extractTableName(const std::string& query) = 0;
};

#endif // !IDATABASE_CONNECTION_H
