#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <mutex>
#include <fstream>
#include <memory>

enum class LogLevel {
	DEBUG = 0,
	INFO,
	WARN,
	ERROR,
	OFF
};

class Logger {
public:
	// 싱글톤 인스턴스 반환
	static Logger& getInstance();

	// 설정 메서드
	void setLogLevel(LogLevel level);
	void setVerbose(bool verbose);
	void enableConsoleOutput(bool enable);
	void enableFileOutput(const std::string& filename);
	void disableFileOutput();

	// 로그 출력 메서드
	void debug(const std::string& message);
	void info(const std::string& message);
	void warn(const std::string& message);
	void error(const std::string& message);

private:
	// 생성자와 소멸자를 private로 선언
	Logger();
	~Logger();

	// 복사 생성자와 대입 연산자를 삭제하여 싱글톤 보장
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	// 내부 사용 메서드
	void log(LogLevel level, const std::string& message);
	std::string getTimestamp();
	std::string levelToString(LogLevel level);

	// 멤버 변수
	LogLevel currentLevel;
	bool verbose;
	bool consoleOutput;
	bool fileOutput;
	std::ofstream fileStream;
	std::mutex mtx;
};

#endif // !LOGGER_H
