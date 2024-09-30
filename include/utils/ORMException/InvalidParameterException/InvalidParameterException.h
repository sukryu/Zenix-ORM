#ifndef INVALID_PARAMETER_EXCEPTION_H
#define INVALID_PARAMETER_EXCEPTION_H

#include "ORMException.h"

class InvalidParameterException : public ORMException {
public:
    explicit InvalidParameterException(const std::string& message);
    virtual ~InvalidParameterException() noexcept = default;
};

#endif // INVALID_PARAMETER_EXCEPTION_H
