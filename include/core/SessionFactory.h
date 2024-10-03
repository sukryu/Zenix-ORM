#ifndef SESSION_FACTORY_H
#define SESSION_FACTORY_H

#include "ConnectionPool.h"
#include "DatabaseConfig.h"
#include <memory>

class SessionFactory {
public:
    static SessionFactory& getInstance();

    void configure(const DatabaseConfig& config);
    std::shared_ptr<IDatabaseConnection> getConnection();
    void releaseConnection(std::shared_ptr<IDatabaseConnection> connection);

private:
    SessionFactory();
    ~SessionFactory();

    SessionFactory(const SessionFactory&) = delete;
    SessionFactory& operator=(const SessionFactory&) = delete;

    std::unique_ptr<ConnectionPool> connectionPool;
};

#endif // SESSION_FACTORY_H
