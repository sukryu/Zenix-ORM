#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <chrono>
#include "IDatabaseConnection.h"
#include "DatabaseConfig.h"

class ConnectionPool {
public:
    ConnectionPool(const DatabaseConfig& config, size_t initialSize, size_t maxSize);
    ~ConnectionPool();

    std::shared_ptr<IDatabaseConnection> acquireConnection();
    void releaseConnection(std::shared_ptr<IDatabaseConnection> connection);

private:
    void createInitialConnections();
    std::shared_ptr<IDatabaseConnection> createNewConnection();

    DatabaseConfig config;
    size_t maxPoolSize;
    std::queue<std::shared_ptr<IDatabaseConnection>> connections;
    std::mutex poolMutex;
    std::condition_variable condition;
    size_t currentPoolSize;
};

#endif // CONNECTION_POOL_H
