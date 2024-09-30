#ifndef SESSION_FACTORY_H
#define SESSION_FACTORY_H

#include <memory>
#include "database/DatabaseConfig.h"
#include "core/ISession.h"
#include "database/DatabaseConnectionFactory.h"
#include "utils/logger/Logger.h"

class SessionFactory {
public:
    static SessionFactory& getInstance();

    void configure(const DatabaseConfig& config);
    std::shared_ptr<ISession> openSession();
    std::shared_ptr<ISession> getCurrentSession();

private:
    SessionFactory();
    SessionFactory(const SessionFactory&) = delete;
    SessionFactory& operator=(const SessionFactory&) = delete;
    thread_local static std::shared_ptr<ISession> currentSession;

    DatabaseConfig config;
    Logger& logger;
    bool isConfigured;
};

#endif // SESSION_FACTORY_H
