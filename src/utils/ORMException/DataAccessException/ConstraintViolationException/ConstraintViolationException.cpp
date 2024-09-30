#include "include/utils/ORMException/DataAccessException/ConstraintViolationException/ConstraintViolationException.h"

ConstraintViolationException::ConstraintViolationException(const std::string& message)
    : DataAccessException("ConstraintViolationException: " + message) {
}
