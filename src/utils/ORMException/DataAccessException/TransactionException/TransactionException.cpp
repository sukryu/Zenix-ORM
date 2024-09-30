#include "include/utils/ORMException/DataAccessException/TransactionException/TransactionException.h"

TransactionException::TransactionException(const std::string& message)
    : DataAccessException("TransactionException: " + message) {
}
