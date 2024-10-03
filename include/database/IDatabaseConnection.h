#ifndef IDATABASE_CONNECTION_H
#define IDATABASE_CONNECTION_H

#include <string>
#include <vector>
#include <map>
#include <any>

class IDatabaseConnection {
public:
    virtual ~IDatabaseConnection() = default;

    // Database connection and disconnection
    virtual void connect() = 0;
    virtual void disconnect() = 0;

    // Execute query with parameters
    virtual std::vector<std::map<std::string, std::string>> executeQuery(
        const std::string& query,
        const std::vector<std::any>& params = {}) = 0;

    // Execute update with parameters
    virtual int executeUpdate(
        const std::string& query,
        const std::vector<std::any>& params = {}) = 0;

    // Transaction management
    virtual void beginTransaction() = 0;
    virtual void commit() = 0;
    virtual void rollback() = 0;

    // Get native handle
    virtual void* getNativeHandle() = 0;

    virtual std::string extractTableName(const std::string& query) = 0;
};

#endif // IDATABASE_CONNECTION_H
