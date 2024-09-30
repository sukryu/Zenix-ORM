#include "include/utils/ORMException/DataAccessException/DatabaseConnectionException/DatabaseConnectionException.h"

DatabaseConnectionException::DatabaseConnectionException(const std::string& message)
    : DataAccessException("DatabaseConnectionException: " + message) {
}
