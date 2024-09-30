#ifndef DATABASE_CONNECTION_EXCEPTION_H
#define DATABASE_CONNECTION_EXCEPTION_H

#include "DataAccessException.h"

class DatabaseConnectionException : public DataAccessException {
public:
    explicit DatabaseConnectionException(const std::string& message);
    virtual ~DatabaseConnectionException() noexcept = default;
};

#endif // DATABASE_CONNECTION_EXCEPTION_H
