#include "ConnectionPool.h"
#include "DatabaseConnectionFactory.h"

ConnectionPool::ConnectionPool(const DatabaseConfig& config, size_t initialSize, size_t maxSize)
    : config(config), maxPoolSize(maxSize), currentPoolSize(0) {
    for (size_t i = 0; i < initialSize; ++i) {
        auto connection = createNewConnection();
        connections.push(connection);
    }
}

ConnectionPool::~ConnectionPool() {
    std::lock_guard<std::mutex> lock(poolMutex);
    while (!connections.empty()) {
        auto connection = connections.front();
        connections.pop();
        connection->disconnect();
    }
}

std::shared_ptr<IDatabaseConnection> ConnectionPool::createNewConnection() {
    auto connection = DatabaseConnectionFactory::createConnection(config);
    connection->connect();
    ++currentPoolSize;
    return connection;
}

std::shared_ptr<IDatabaseConnection> ConnectionPool::acquireConnection() {
    std::unique_lock<std::mutex> lock(poolMutex);

    auto timeout = std::chrono::steady_clock::now() + std::chrono::seconds(10);
    while (connections.empty()) {
        if (currentPoolSize < maxPoolSize) {
            // 동적으로 새로운 연결을 생성합니다.
            auto connection = createNewConnection();
            return connection;
        } else {
            if (condition.wait_until(lock, timeout) == std::cv_status::timeout) {
                throw std::runtime_error("Failed to acquire connection: Timeout after 10 seconds.");
            }
        }
    }

    auto connection = connections.front();
    connections.pop();
    return connection;
}

void ConnectionPool::releaseConnection(std::shared_ptr<IDatabaseConnection> connection) {
    std::lock_guard<std::mutex> lock(poolMutex);
    connections.push(connection);
    condition.notify_one();
}
