#ifndef LOCK_ACQUISITION_EXCEPTION_H
#define LOCK_ACQUISITION_EXCEPTION_H

#include "DataAccessException.h"

class LockAcquisitionException : public DataAccessException {
public:
    explicit LockAcquisitionException(const std::string& message);
    virtual ~LockAcquisitionException() noexcept = default;
};

#endif // LOCK_ACQUISITION_EXCEPTION_H
