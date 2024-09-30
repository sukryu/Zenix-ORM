#ifndef OPTIMISTIC_LOCK_EXCEPTION_H
#define OPTIMISTIC_LOCK_EXCEPTION_H

#include "DataAccessException.h"

class OptimisticLockException : public DataAccessException {
public:
    explicit OptimisticLockException(const std::string& message);
    virtual ~OptimisticLockException() noexcept = default;
};

#endif // OPTIMISTIC_LOCK_EXCEPTION_H
