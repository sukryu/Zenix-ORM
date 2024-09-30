#ifndef CONSTRAINT_VIOLATION_EXCEPTION_H
#define CONSTRAINT_VIOLATION_EXCEPTION_H

#include "DataAccessException.h"

class ConstraintViolationException : public DataAccessException {
public:
    explicit ConstraintViolationException(const std::string& message);
    virtual ~ConstraintViolationException() noexcept = default;
};

#endif // CONSTRAINT_VIOLATION_EXCEPTION_H
