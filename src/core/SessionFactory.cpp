#include "SessionFactory.h"

SessionFactory::SessionFactory() {}

SessionFactory::~SessionFactory() {}

SessionFactory& SessionFactory::getInstance() {
    static SessionFactory instance;
    return instance;
}

void SessionFactory::configure(const DatabaseConfig& config) {
    // 초기 풀 크기와 최대 풀 크기를 설정합니다.
    size_t initialPoolSize = 5;
    size_t maxPoolSize = 20;

    connectionPool = std::make_unique<ConnectionPool>(config, initialPoolSize, maxPoolSize);
}

std::shared_ptr<IDatabaseConnection> SessionFactory::getConnection() {
    return connectionPool->acquireConnection();
}

void SessionFactory::releaseConnection(std::shared_ptr<IDatabaseConnection> connection) {
    connectionPool->releaseConnection(connection);
}
