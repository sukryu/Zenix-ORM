#include "core/SessionFactory.h"
#include "ORMException/ConfigurationException/ConfigurationException.h"
#include "core/Session.h"

SessionFactory& SessionFactory::getInstance() {
    static SessionFactory instance;
    return instance;
}

SessionFactory::SessionFactory()
    : logger(Logger::getInstance()), isConfigured(false) {
    logger.debug("SessionFactory created.");
}

void SessionFactory::configure(const DatabaseConfig& config) {
    this->config = config;
    isConfigured = true;
    logger.info("SessionFactory configured.");
}

std::shared_ptr<ISession> SessionFactory::openSession() {
    if (!isConfigured) {
        throw ConfigurationException("SessionFactory is not configured.");
    }

    auto connection = DatabaseConnectionFactory::createConnection(config);
    connection->connect();
    logger.debug("Opening new session.");
    return std::make_shared<Session>(connection);
}

thread_local std::shared_ptr<ISession> SessionFactory::currentSession = nullptr;

std::shared_ptr<ISession> SessionFactory::getCurrentSession() {
    if (!currentSession) {
        currentSession = openSession();
    }
    return currentSession;
}