#include "include/database/DatabaseConnectionFactory.h"
#include "include/utils/ORMException/ConfigurationException/ConfigurationException.h"
#include "database/DatabaseConfig.h"

std::shared_ptr<IDatabaseConnection> DatabaseConnectionFactory::createConnection(const DatabaseConfig& config) {
    if (config.getDatabaseType() == DatabaseType::SQLite) {
        return std::make_shared<SQLiteConnection>(config);
    } else {
        throw ConfigurationException("Unsupported database type: " + std::to_string(static_cast<int>(config.getDatabaseType())));
    }
}
