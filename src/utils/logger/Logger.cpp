#include "include/utils/logger/Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

Logger& Logger::getInstance() {
	static Logger instance;
	return instance;
}

Logger::Logger()
	: currentLevel(LogLevel::DEBUG),
	verbose(false),
	consoleOutput(true),
	fileOutput(false) {
}

Logger::~Logger() {
	if (fileStream.is_open()) {
		fileStream.close();
	}
}

void Logger::setLogLevel(LogLevel level) {
	currentLevel = level;
}

void Logger::setVerbose(bool verbose) {
	this->verbose = verbose;
}

void Logger::enableConsoleOutput(bool enable) {
	consoleOutput = enable;
}

void Logger::enableFileOutput(const std::string& filename) {
	std::lock_guard<std::mutex> lock(mtx);
	if (fileStream.is_open()) {
		fileStream.close();
	}
	fileStream.open(filename, std::ios::out | std::ios::app);
	if (fileStream.is_open()) {
		fileOutput = true;
	}
	else {
		// 파일 열기에 실패한 경우 처리
		consoleOutput = true;
		error("Failed to open log file: ", + filename);
	}
}

void Logger::disableFileOutput() {
	std::lock_guard<std::mutex> lock(mtx);
	if (fileStream.is_open()) {
		fileStream.close();
	}
	fileOutput = false;
}

void Logger::debug(const std::string& message) {
	if (currentLevel <= LogLevel::DEBUG) {
		log(LogLevel::DEBUG, message);
	}
}

void Logger::info(const std::string& message) {
	if (currentLevel <= LogLevel::INFO) {
		log(LogLevel::INFO, message);
	}
}

void Logger::warn(const std::string& message) {
	if (currentLevel <= LogLevel::WARN) {
		log(LogLevel::WARN, message);
	}
}

void Logger::error(const std::string& message) {
	if (currentLevel <= LogLevel::ERROR) {
		log(LogLevel::ERROR, message);
	}
}

void Logger::log(LogLevel level, const std::string& message) {
	std::lock_guard<std::mutex> lock(mtx);

	std::string logMessage;
	if (verbose) {
		logMessage = "[" + getTimestamp() + "] [" + levelToString(level) + "] " + message;
	}
	else {
		logMessage = "[" + levelToString(level) + "] " + message;
	}

	if (consoleOutput) {
		std::cout << logMessage << std::endl;
	}

	if (fileOutput && fileStream.is_open()) {
		fileStream << logMessage << std::endl;
	}
}

std::string Logger::getTimestamp() {
	auto now = std::chrono::system_clock::now();
	auto timeT = std::chrono::system_clock::to_time_t(now);
	auto localTime = std::localtime(&timeT);

	std::ostringstream ss;
	ss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
	return ss.str();
}

std::string Logger::levelToString(LogLevel level) {
	switch (level) {
	case LogLevel::DEBUG:
		return "DEBUG";
	case LogLevel::INFO:
		return "INFO ";
	case LogLevel::WARN:
		return "WARN ";
	case LogLevel::ERROR:
		return "ERROR";
	default:
		return "UNKNOWN";
	}
}
