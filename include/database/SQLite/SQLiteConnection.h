#ifndef SQLITE_CONNECTION_H
#define SQLITE_CONNECTION_H

#include "IDatabaseConnection.h"
#include "DatabaseConfig.h"
#include "../utils/logger/Logger.h"
#include <sqlite3.h>
#include <any>
#include <mutex>

class SQLiteConnection : public IDatabaseConnection {
public:
    explicit SQLiteConnection(const DatabaseConfig& config);
    virtual ~SQLiteConnection();

    // Database connection and disconnection
    void connect() override;
    void disconnect() override;

    // Execute query with parameters
    std::vector<std::map<std::string, std::string>> executeQuery(
        const std::string& query,
        const std::vector<std::any>& params = {}) override;

    // Execute update with parameters
    int executeUpdate(
        const std::string& query,
        const std::vector<std::any>& params = {}) override;

    // Transaction management
    void beginTransaction() override;
    void commit() override;
    void rollback() override;

    // Get native handle
    void* getNativeHandle() override;

    std::string extractTableName(const std::string& query) override;

private:
    sqlite3* db;
    DatabaseConfig config;
    Logger& logger;
    bool isConnected;
    bool isInTransaction;
    std::mutex connectionMutex;

    void finalizeStatement(sqlite3_stmt* stmt);
    void bindParameters(sqlite3_stmt* stmt, const std::vector<std::any>& params);
};

#endif // SQLITE_CONNECTION_H
