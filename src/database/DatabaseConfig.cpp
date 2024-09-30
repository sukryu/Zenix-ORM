#include "include/database/DatabaseConfig.h"

DatabaseConfig::DatabaseConfig()
    : dbType(DatabaseType::SQLite), dbName("default.db") {
}

void DatabaseConfig::setDatabaseType(DatabaseType type) {
    dbType = type;
}

void DatabaseConfig::setDatabaseName(const std::string& name) {
    dbName = name;
}

DatabaseType DatabaseConfig::getDatabaseType() const {
    return dbType;
}

std::string DatabaseConfig::getDatabaseName() const {
    return dbName;
}
