#ifndef DATA_ACCESS_EXCEPTION_H
#define DATA_ACCESS_EXCEPTION_H

#include "ORMException.h"

class DataAccessException : public ORMException {
public:
    explicit DataAccessException(const std::string& message);
    virtual ~DataAccessException() noexcept = default;
};

#endif // DATA_ACCESS_EXCEPTION_H
