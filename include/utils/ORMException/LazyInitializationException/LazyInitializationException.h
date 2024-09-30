#ifndef LAZY_INITIALIZATION_EXCEPTION_H
#define LAZY_INITIALIZATION_EXCEPTION_H

#include "ORMException.h"

class LazyInitializationException : public ORMException {
public:
    explicit LazyInitializationException(const std::string& message);
    virtual ~LazyInitializationException() noexcept = default;
};

#endif // LAZY_INITIALIZATION_EXCEPTION_H
