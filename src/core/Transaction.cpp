#include "core/Transaction.h"
#include "ORMException/DataAccessException/TransactionException/TransactionException.h"

Transaction::Transaction(std::shared_ptr<IDatabaseConnection> connection, bool& transactionFlag)
    : connection(connection), logger(Logger::getInstance()), isTransactionActive(transactionFlag), isCommittedOrRolledBack(false) {
        logger.debug("Transaction created.");
}

Transaction::~Transaction() {
    if (!isCommittedOrRolledBack && isTransactionActive) {
        try {
            rollback();
            logger.warn("Transaction was not committed. Rolled back.");
        } catch (const TransactionException& e) {
            logger.error(e.what());
        }
    }

    logger.debug("Transaction destroyed.");
}

void Transaction::commit() {
    if (!isTransactionActive) {
        throw TransactionException("No active transaction to commit.");
    }

    try {
        connection->commit();
        isTransactionActive = false;
        isCommittedOrRolledBack = true;
        logger.info("Transaction committed.");
    } catch (const TransactionException& e) {
        logger.error(e.what());
        throw;
    }
}

void Transaction::rollback() {
    if (!isTransactionActive) {
        throw TransactionException("No active transaction to rollback.");
    }

    try {
        connection->rollback();
        isTransactionActive = false;
        isCommittedOrRolledBack = true;
        logger.info("Transaction rolled back.");
    } catch (const TransactionException& e) {
        logger.error(e.what());
        throw;
    }
}