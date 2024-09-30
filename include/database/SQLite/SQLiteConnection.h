#ifndef SQLITE_CONNECTION_H
#define SQLITE_CONNECTION_H

#include "include/database/IDatabaseConnection.h"
#include "../utils/DatabaseConfig.h"
#include <sqlite3.h>

class SQLiteConnection : public IDatabaseConnection {
public:
	explicit SQLiteConnection(const DatabaseConfig& config);
	virtual ~SQLiteConnection();

	// 데이터베이스 연결 및 해제
	void connect() override;
	void disconnect() override;

	// 쿼리 실행
	std::vector<std::map<std::string, std::string>> executeQuery(const std::string& query) override;
	int executeUpdate(const std::string& query) override;

	// 트랜잭션 관리
	void beginTransaction() override;
	void commit() override;
	void rollback() override;

	sqlite3* getNativeHandle() override;
	std::string extractTableName(const std::string& query) override;

private:
	sqlite3* db;
	DatabaseConfig config;
	Logger& logger;
	bool isConnected;
};


#endif // !SQLITE_CONNECTION_H
