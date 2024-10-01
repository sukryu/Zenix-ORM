#ifndef SQLITE_CONNECTION_H
#define SQLITE_CONNECTION_H

#include "include/database/IDatabaseConnection.h"
#include "../utils/DatabaseConfig.h"
#include <sqlite3.h>
#include <any>

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
    sqlite3* getNativeHandle() override;

    std::string extractTableName(const std::string& query) override;

private:
    sqlite3* db;
    DatabaseConfig config;
    Logger& logger;
    bool isConnected;
};


#endif // !SQLITE_CONNECTION_H
