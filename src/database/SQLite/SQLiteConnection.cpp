#include "include/database/SQLite/SQLiteConnection.h"
#include "../DatabaseConnectionException/DatabaseConnectionException.h"
#include "../QueryExecutionException/QueryExecutionException.h"
#include "../TransactionException/TransactionException.h"
#include "database/DatabaseConfig.h"
#include <regex>

SQLiteConnection::SQLiteConnection(const DatabaseConfig& config)
    : db(nullptr), config(config), logger(Logger::getInstance()),
      isConnected(false), isInTransaction(false) {}

SQLiteConnection::~SQLiteConnection() {
    disconnect();
}

void SQLiteConnection::connect() {
    std::lock_guard<std::mutex> lock(connectionMutex);
    if (isConnected) {
        logger.warn("Already connected to the database.");
        return;
    }
    int rc = sqlite3_open(config.getDatabaseName().c_str(), &db);
    if (rc != SQLITE_OK) {
        std::string errorMessage = sqlite3_errmsg(db);
        logger.error("Failed to connect to SQLite database: " + errorMessage);
        throw DatabaseConnectionException(errorMessage);
    }
    isConnected = true;
    logger.info("Connected to SQLite database: " + config.getDatabaseName());
}

void SQLiteConnection::disconnect() {
    std::lock_guard<std::mutex> lock(connectionMutex);
    if (isConnected && db) {
        sqlite3_close(db);
        db = nullptr;
        isConnected = false;
        logger.info("Disconnected from SQLite database.");
    }
}

void SQLiteConnection::finalizeStatement(sqlite3_stmt* stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}

void SQLiteConnection::bindParameters(sqlite3_stmt* stmt, const std::vector<std::any>& params) {
    for (size_t i = 0; i < params.size(); ++i) {
        int index = static_cast<int>(i + 1);
        const std::any& param = params[i];
        if (param.type() == typeid(int)) {
            sqlite3_bind_int(stmt, index, std::any_cast<int>(param));
        } else if (param.type() == typeid(double)) {
            sqlite3_bind_double(stmt, index, std::any_cast<double>(param));
        } else if (param.type() == typeid(std::string)) {
            const std::string& strValue = std::any_cast<std::string>(param);
            sqlite3_bind_text(stmt, index, strValue.c_str(), -1, SQLITE_TRANSIENT);
        } else if (param.type() == typeid(nullptr)) {
            sqlite3_bind_null(stmt, index);
        } else {
            logger.error("Unsupported parameter type at index " + std::to_string(index));
            throw QueryExecutionException("Unsupported parameter type at index " + std::to_string(index));
        }
    }
}

std::vector<std::map<std::string, std::string>> SQLiteConnection::executeQuery(
    const std::string& query, const std::vector<std::any>& params) {
    std::lock_guard<std::mutex> lock(connectionMutex);
    logger.debug("Executing query: " + query);

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::string errorMessage = sqlite3_errmsg(db);
        logger.error("Failed to prepare statement: " + errorMessage);
        finalizeStatement(stmt);
        throw QueryExecutionException(errorMessage);
    }

    try {
        bindParameters(stmt, params);

        std::vector<std::map<std::string, std::string>> results;
        int columnCount = sqlite3_column_count(stmt);

        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            std::map<std::string, std::string> row;
            for (int i = 0; i < columnCount; ++i) {
                std::string columnName = sqlite3_column_name(stmt, i);
                const unsigned char* text = sqlite3_column_text(stmt, i);
                std::string value = text ? reinterpret_cast<const char*>(text) : "";
                row[columnName] = value;
            }
            results.push_back(row);
        }

        if (rc != SQLITE_DONE) {
            std::string errorMessage = sqlite3_errmsg(db);
            logger.error("Failed to execute query: " + errorMessage);
            throw QueryExecutionException(errorMessage);
        }

        logger.debug("Query executed successfully.");
        return results;
    } catch (...) {
        finalizeStatement(stmt);
        throw;
    }

    finalizeStatement(stmt);
}

int SQLiteConnection::executeUpdate(const std::string& query, const std::vector<std::any>& params) {
    std::lock_guard<std::mutex> lock(connectionMutex);
    logger.debug("Executing update: " + query);

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::string errorMessage = sqlite3_errmsg(db);
        logger.error("Failed to prepare statement: " + errorMessage);
        finalizeStatement(stmt);
        throw QueryExecutionException(errorMessage);
    }

    try {
        bindParameters(stmt, params);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::string errorMessage = sqlite3_errmsg(db);
            logger.error("Failed to execute update: " + errorMessage);
            throw QueryExecutionException(errorMessage);
        }

        int affectedRows = sqlite3_changes(db);
        logger.debug("Update executed successfully. Rows affected: " + std::to_string(affectedRows));
        return affectedRows;
    } catch (...) {
        finalizeStatement(stmt);
        throw;
    }

    finalizeStatement(stmt);
}

void SQLiteConnection::beginTransaction() {
    std::lock_guard<std::mutex> lock(connectionMutex);
    if (isInTransaction) {
        logger.error("Transaction already in progress.");
        throw TransactionException("Transaction already in progress.");
    }
    int rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::string errorMessage = sqlite3_errmsg(db);
        logger.error("Failed to begin transaction: " + errorMessage);
        throw TransactionException(errorMessage);
    }
    isInTransaction = true;
    logger.debug("Transaction started.");
}

void SQLiteConnection::commit() {
    std::lock_guard<std::mutex> lock(connectionMutex);
    if (!isInTransaction) {
        logger.error("No transaction in progress to commit.");
        throw TransactionException("No transaction in progress to commit.");
    }
    int rc = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::string errorMessage = sqlite3_errmsg(db);
        logger.error("Failed to commit transaction: " + errorMessage);
        throw TransactionException(errorMessage);
    }
    isInTransaction = false;
    logger.debug("Transaction committed.");
}

void SQLiteConnection::rollback() {
    std::lock_guard<std::mutex> lock(connectionMutex);
    if (!isInTransaction) {
        logger.error("No transaction in progress to rollback.");
        throw TransactionException("No transaction in progress to rollback.");
    }
    int rc = sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::string errorMessage = sqlite3_errmsg(db);
        logger.error("Failed to rollback transaction: " + errorMessage);
        throw TransactionException(errorMessage);
    }
    isInTransaction = false;
    logger.debug("Transaction rolled back.");
}

void* SQLiteConnection::getNativeHandle() {
    return static_cast<void*>(db);
}

std::string SQLiteConnection::extractTableName(const std::string& query) {
    std::regex fromRegex("FROM\\s+([a-zA-Z0-9_]+)", std::regex::icase);
    std::smatch match;
    if (std::regex_search(query, match, fromRegex)) {
        return match[1].str();
    } else {
        return "";
    }
}