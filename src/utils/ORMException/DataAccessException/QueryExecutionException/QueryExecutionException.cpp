#include "include/utils/ORMException/DataAccessException/QueryExecutionException/QueryExecutionException.h"

QueryExecutionException::QueryExecutionException(const std::string& message)
    : DataAccessException("QueryExecutionException: " + message) {
}
