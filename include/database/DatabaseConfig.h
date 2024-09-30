#ifndef DATABASE_CONFIG_H
#define DATABASE_CONFIG_H

#include <string>

enum class DatabaseType {
    SQLite,
    // 향후 다른 데이터베이스 타입 추가 가능
};

class DatabaseConfig {
public:
    DatabaseConfig();

    // 설정 메서드
    void setDatabaseType(DatabaseType type);
    void setDatabaseName(const std::string& name);

    // Getter 메서드
    DatabaseType getDatabaseType() const;
    std::string getDatabaseName() const;

private:
    DatabaseType dbType;
    std::string dbName;
};

#endif // DATABASE_CONFIG_H
