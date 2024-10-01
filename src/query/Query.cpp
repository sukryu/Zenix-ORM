#include "query/Query.h"
#include <regex>
#include "ORMException/DataAccessException/QueryExecutionException/QueryExecutionException.h"
#include "ORMException/MappingException/MappingException.h"

Query::Query(std::shared_ptr<IDatabaseConnection> connection, const std::string& queryString)
    : connection(connection), queryString(queryString), logger(Logger::getInstance()), stmt(nullptr) {
    logger.debug("Query created with query string: " + queryString);
}

Query::~Query() {
    if (stmt) {
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    logger.debug("Query destroyed.");
}

void Query::setParameter(const std::string& name, const std::string& value) {
    parameters[name] = value;
    logger.debug("Parameter set: " + name + " = " + value);
}

void Query::prepareAndBind() {
    std::lock_guard<std::mutex> lock(cacheMutex);

    if (stmt) {
        sqlite3_reset(stmt);
    } else {
        // 캐시에서 Prepared Statement 검색
        auto it = statementCache.find(queryString);
        if (it != statementCache.end()) {
            stmt = it->second;
            sqlite3_reset(stmt);
        } else {
            sqlite3* db = connection->getNativeHandle();
            int rc = sqlite3_prepare_v2(db, queryString.c_str(), -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                std::string errorMessage = sqlite3_errmsg(db);
                logger.error("Failed to prepare statement: " + errorMessage);
                throw QueryExecutionException(errorMessage);
            }
            statementCache[queryString] = stmt;
        }
    }

    // 모든 파라미터를 바인딩
    int paramCount = sqlite3_bind_parameter_count(stmt);
    for (int i = 0; i < paramCount; i++) {
        const char* paramName = sqlite3_bind_parameter_name(stmt, i);
        if (paramName && paramName[0] == ':') {
            std::string name = paramName + 1; // ':' 제거
            auto it = parameters.find(name);
            if (it != parameters.end()) {
                sqlite3_bind_text(stmt, i, it->second.c_str(), -1, SQLITE_TRANSIENT);
            } else {
                throw QueryExecutionException("Parameter :" + name + "is not set.");
            }
        } else {
            throw QueryExecutionException("Unnamed parameters are not supported");
        }
    }
}

void Query::finalizeStatement() {
    std::lock_guard<std::mutex> lock(cacheMutex);

    // Prepared Statement를 캐시에서 제거하지 않고 유지하여 재사용
    stmt = nullptr;
}

std::vector<std::shared_ptr<IEntity>> Query::list() {
    prepareAndBind();

    // 결과 처리 (엔티티 매핑)
    std::vector<std::shared_ptr<IEntity>> entities;

    // 테이블 이름 추출 및 매핑 정보 가져오기
    std::string tableName = connection->extractTableName(queryString);
    auto mappingInfo = EntityMapper::getInstance().getMappingByTableName(tableName);
    if (!mappingInfo) {
        throw MappingException("No mapping found for table: " + tableName);
    }

    int rc;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        auto entity = mappingInfo->entityConstructor();
        for (int i = 0; i < sqlite3_column_count(stmt); ++i) {
            std::string columnName = sqlite3_column_name(stmt, i);
            const unsigned char* text = sqlite3_column_text(stmt, i);
            std::string value = text ? reinterpret_cast<const char*>(text) : "";

            if (columnName == mappingInfo->idColumnName) {
                entity->setId(value);
            } else {
                // 필드 매핑 찾아서 값 설정
                for (const auto& field : mappingInfo->fields) {
                    if (field.columnName == columnName) {
                        entity->setFieldValue(field.fieldName, value);
                        break;
                    }
                }
            }
        }
        entities.push_back(entity);
    }

    if (rc != SQLITE_DONE) {
        std::string errorMessage = sqlite3_errmsg(connection->getNativeHandle());
        sqlite3_finalize(stmt);
        stmt = nullptr;
        logger.error("Failed to execute query: " + errorMessage);
        throw QueryExecutionException(errorMessage);
    }

    sqlite3_finalize(stmt);
    stmt = nullptr;

    logger.debug("Query executed successfully. Rows fetched: " + std::to_string(entities.size()));
    return entities;
}

std::shared_ptr<IEntity> Query::uniqueResult() {
    auto results = list();
    if (results.size() == 1) {
        return results[0];
    } else if (results.empty()) {
        return nullptr;
    } else {
        throw QueryExecutionException("Query returned more than one result.");
    }
}

std::vector<std::map<std::string, std::string>> Query::listMap() {
    prepareAndBind();

    // 결과 처리 (맵으로 반환)
    std::vector<std::map<std::string, std::string>> results;

    int rc;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::map<std::string, std::string> row;
        for (int i = 0; i < sqlite3_column_count(stmt); ++i) {
            std::string columnName = sqlite3_column_name(stmt, i);
            const unsigned char* text = sqlite3_column_text(stmt, i);
            std::string value = text ? reinterpret_cast<const char*>(text) : "";
            row[columnName] = value;
        }
        results.push_back(row);
    }

    if (rc != SQLITE_DONE) {
        std::string errorMessage = sqlite3_errmsg(connection->getNativeHandle());
        sqlite3_finalize(stmt);
        stmt = nullptr;
        logger.error("Failed to execute query: " + errorMessage);
        throw QueryExecutionException(errorMessage);
    }

    sqlite3_finalize(stmt);
    stmt = nullptr;

    logger.debug("Query executed successfully. Rows fetched: " + std::to_string(results.size()));
    return results;
}