#ifndef TRANSACTION_EXCEPTION_H
#define TRANSACTION_EXCEPTION_H

#include "DataAccessException.h"

class TransactionException : public DataAccessException {
public:
    explicit TransactionException(const std::string& message);
    virtual ~TransactionException() noexcept = default;
};

#endif // TRANSACTION_EXCEPTION_H
