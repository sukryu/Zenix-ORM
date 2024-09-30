#ifndef QUERY_EXECUTION_EXCEPTION_H
#define QUERY_EXECUTION_EXCEPTION_H

#include "DataAccessException.h"

class QueryExecutionException : public DataAccessException {
public:
    explicit QueryExecutionException(const std::string& message);
    virtual ~QueryExecutionException() noexcept = default;
};

#endif // QUERY_EXECUTION_EXCEPTION_H
