#include "include/database/SQLite/SQLiteConnection.h"
#include "../DatabaseConnectionException/DatabaseConnectionException.h"
#include "../QueryExecutionException/QueryExecutionException.h"
#include "../TransactionException/TransactionException.h"

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
		logger.error("Failed to connect to SQLite database: ", + errorMessage);
		throw DatabaseConnectionException(errorMessage);
	}
	isConnected = true;
	logger.info("Connected to SQLite database: ", + config.getDatabaseName());
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
}
