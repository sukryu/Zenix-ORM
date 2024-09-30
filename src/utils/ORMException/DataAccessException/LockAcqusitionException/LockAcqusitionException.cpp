#include "include/utils/ORMException/DataAccessException/LockAcquisitionException/LockAcqusitionException.h"

LockAcquisitionException::LockAcquisitionException(const std::string& message)
    : DataAccessException("LockAcquisitionException: " + message) {
}
