#ifndef DATABASE_CONNECTION_FACTORY_H
#define DATABASE_CONNECTION_FACTORY_H

#include "IDatabaseConnection.h"
#include "SQLiteConnection.h"
#include "../utils/DatabaseConfig.h"

class DatabaseConnectionFactory {
public:
    static std::shared_ptr<IDatabaseConnection> createConnection(const DatabaseConfig& config);
};

#endif // DATABASE_CONNECTION_FACTORY_H