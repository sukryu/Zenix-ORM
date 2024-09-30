#include "include/utils/ORMException/DataAccessException/DataAccessException.h"

DataAccessException::DataAccessException(const std::string& message)
    : ORMException("DataAccessException: " + message) {
}
