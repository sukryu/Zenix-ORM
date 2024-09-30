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

std::vector<std::map<std::string, std::string>> SQLiteConnection::executeQuery(const std::string& query) {
	logger.debug("Executing query: " + query);
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		std::string errorMessage = sqlite3_errmsg(db);
		logger.error("Failed to prepare query: " + errorMessage);
		throw QueryExecutionException(errorMessage);
	}

	std::vector<std::map<std::string, std::string>> results;
	int columnCount = sqlite3_column_count(stmt);

	while ((rc == sqlite3_step(stmt)) == SQLITE_ROW) {
		std::map<std::string, std::string> row;
		for (int i = 0; i < columnCount; i++) {
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
	logger.debug("Query executed successfully");
	return results;
}

int SQLiteConnection::executeUpdate(const std::string& query) {
	logger.debug("Executing update: " + query);
	char* errMsg = nullptr;
	int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		std::string errorMessage = errMsg ? errMsg : "Unknown error";
		sqlite3_free(errMsg);
		logger.error("Failed to execute update: " + errorMessage);
		throw QueryExecutionException(errorMessage);
	}

	int affectedRows = sqlite3_changes(db);
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