#include "include/database/SQLite/SQLiteConnection.h"
#include "../DatabaseConnectionException/DatabaseConnectionException.h"
#include "../QueryExecutionException/QueryExecutionException.h"
#include "../TransactionException/TransactionException.h"
#include "database/DatabaseConfig.h"
#include <regex>

SQLiteConnection::SQLiteConnection(const DatabaseConfig& config)
	: db(nullptr), config(config), logger(Logger::getInstance()), isConnected(false) {
}

SQLiteConnection::~SQLiteConnection() {
	disconnect();
}

void SQLiteConnection::connect() {
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
	if (isConnected && db) {
		sqlite3_close(db);
		db = nullptr;
		isConnected = false;
		logger.info("Disconnecting from SQLite database.");
	}
}

std::vector<std::map<std::string, std::string>> SQLiteConnection::executeQuery(
    const std::string& query, const std::vector<std::any>& params) {
    logger.debug("Executing query: " + query);
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::string errorMessage = sqlite3_errmsg(db);
        logger.error("Failed to prepare statement: " + errorMessage);
        sqlite3_finalize(stmt);
        throw QueryExecutionException(errorMessage);
    }

    // Bind parameters
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
        } else {
            sqlite3_bind_null(stmt, index);
        }
    }

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
        sqlite3_finalize(stmt);
        logger.error("Failed to execute query: " + errorMessage);
        throw QueryExecutionException(errorMessage);
    }

    sqlite3_finalize(stmt);
    logger.debug("Query executed successfully.");
    return results;
}

int SQLiteConnection::executeUpdate(const std::string& query, const std::vector<std::any>& params) {
    logger.debug("Executing update: " + query);
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::string errorMessage = sqlite3_errmsg(db);
        logger.error("Failed to prepare statement: " + errorMessage);
        sqlite3_finalize(stmt);
        throw QueryExecutionException(errorMessage);
    }

    // Bind parameters
    for (size_t i = 0; i < params.size(); ++i) {
        int index = static_cast<int>(i + 1); // Bind indices start at 1
        const std::any& param = params[i];
        if (param.type() == typeid(int)) {
            sqlite3_bind_int(stmt, index, std::any_cast<int>(param));
        } else if (param.type() == typeid(double)) {
            sqlite3_bind_double(stmt, index, std::any_cast<double>(param));
        } else if (param.type() == typeid(std::string)) {
            const std::string& strValue = std::any_cast<std::string>(param);
            sqlite3_bind_text(stmt, index, strValue.c_str(), -1, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, index);
        }
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::string errorMessage = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        logger.error("Failed to execute update: " + errorMessage);
        throw QueryExecutionException(errorMessage);
    }

    int affectedRows = sqlite3_changes(db);
    sqlite3_finalize(stmt);
    logger.debug("Update executed successfully. Rows affected: " + std::to_string(affectedRows));
    return affectedRows;
}

void SQLiteConnection::beginTransaction() {
	logger.debug("Beginning transaction");
	int rc = sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
	if (rc != SQLITE_OK) {
		std::string errorMessage = sqlite3_errmsg(db);
		logger.error("Failed to begin transaction: " + errorMessage);
		throw TransactionException(errorMessage);
	}
}

void SQLiteConnection::commit() {
	logger.debug("Committing transaction.");
	int rc = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
	if (rc != SQLITE_OK) {
		std::string errorMessage = sqlite3_errmsg(db);
		logger.error("Failed to commit transaction: " + errorMessage);
		throw TransactionException(errorMessage);
	}
}

void SQLiteConnection::rollback() {
	logger.debug("Rolling back transaction");
	int rc = sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
	if (rc != SQLITE_OK) {
		std::string errorMessage = sqlite3_errmsg(db);
		logger.error("Failed to rollback transaction: " + errorMessage);
		throw TransactionException(errorMessage);
	}
}

sqlite3* SQLiteConnection::getNativeHandle() {
    return db;
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