#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "ITransaction.h"
#include "database/DatabaseConnectionFactory.h"
#include "utils/logger/Logger.h"

class Transaction : public ITransaction {
public:
    Transaction(std::shared_ptr<IDatabaseConnection> connection, bool& transactionFlag);
    virtual ~Transaction();

    void commit() override;
    void rollback() override;

private:
    std::shared_ptr<IDatabaseConnection> connection;
    Logger& logger;
    bool& isTransactionActive;
    bool isCommittedOrRolledBack;
};

#endif // TRANSACTION_H