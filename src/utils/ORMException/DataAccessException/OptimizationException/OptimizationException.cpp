#include "include/utils/ORMException/DataAccessException/OptimizationException/OptimizationException.h"

OptimisticLockException::OptimisticLockException(const std::string& message)
    : DataAccessException("OptimisticLockException: " + message) {
}
