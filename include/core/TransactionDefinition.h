#ifndef TRANSACTION_DEFINITION_H
#define TRANSACTION_DEFINITION_H

enum class IsolationLevel {
    DEFAULT, // SQLite에서는 DEFERRED와 동일
    READ_UNCOMMITTED,
    READ_COMMITTED,
    REPEATABLE_READ,
    SERIALIZABLE,
};

enum class TransactionMode {
    DEFERRED,
    IMMEDIATE,
    EXCLUSIVE,
};

struct TransactionDefinition
{
    IsolationLevel isolationLevel = IsolationLevel::DEFAULT;
    TransactionMode transactionMode = TransactionMode::DEFERRED;
};


#endif // TRANSACTION_DEFINITION_H