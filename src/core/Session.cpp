// src/core/Session.cpp

#include "core/Session.h"
#include "ORMException/ORMException.h"
#include "ORMException/DataAccessException/TransactionException/TransactionException.h"
#include "ORMException/DataAccessException/QueryExecutionException/QueryExecutionException.h"
#include "ORMException/InvalidParameterException/InvalidParameterException.h"
#include "ORMException/MappingException/EntityNotFoundException/EntityNotFoundException.h"
#include "ORMException/MappingException/MappingException.h"
#include "cache/CacheManager.h"

Session::Session(std::shared_ptr<IDatabaseConnection> connection)
    : connection(connection), logger(Logger::getInstance()), isTransactionActive(false) {
    logger.debug("Session created.");
}

Session::~Session() {
    close();
    logger.debug("Session destroyed.");
}

void Session::save(std::shared_ptr<IEntity> entity) {
    if (!entity) {
        throw InvalidParameterException("Entity cannot be null.");
    }

    logger.debug("Saving entity: " + entity->getEntityName());

    // 엔티티 매핑 정보를 가져옴
    auto mappingInfo = EntityMapper::getInstance().getMapping(entity->getEntityName());
    if (!mappingInfo) {
        throw MappingException("No mapping information found for entity: " + entity->getEntityName());
    }

    // INSERT 쿼리 생성
    std::string query = "INSERT INTO " + mappingInfo->tableName + " (";
    std::string values = "VALUES (";

    for (const auto& field : mappingInfo->fields) {
        query += field.columnName + ", ";
        values += "'" + entity->getFieldValue(field.fieldName) + "', ";
    }

    // 마지막 콤마와 공백 제거
    query = query.substr(0, query.size() - 2) + ") ";
    values = values.substr(0, values.size() - 2) + ");";
    query += values;

    try {
        connection->executeUpdate(query);
        logger.info("Entity saved successfully.");
    } catch (const QueryExecutionException& e) {
        logger.error(e.what());
        throw;
    }
}

void Session::update(std::shared_ptr<IEntity> entity) {
    if (!entity) {
        throw InvalidParameterException("Entity cannot be null.");
    }

    logger.debug("Updating entity: " + entity->getEntityName());

    auto mappingInfo = EntityMapper::getInstance().getMapping(entity->getEntityName());
    if (!mappingInfo) {
        throw MappingException("No mapping information found for entity: " + entity->getEntityName());
    }

    // UPDATE 쿼리 생성
    std::string query = "UPDATE " + mappingInfo->tableName + " SET ";

    for (const auto& field : mappingInfo->fields) {
        query += field.columnName + " = '" + entity->getFieldValue(field.fieldName) + "', ";
    }

    // 마지막 콤마와 공백 제거
    query = query.substr(0, query.size() - 2);

    // WHERE 절 추가
    query += " WHERE " + mappingInfo->idColumnName + " = '" + entity->getId() + "';";

    try {
        connection->executeUpdate(query);
        logger.info("Entity updated successfully.");
    } catch (const QueryExecutionException& e) {
        logger.error(e.what());
        throw;
    }
}

void Session::remove(std::shared_ptr<IEntity> entity) {
    if (!entity) {
        throw InvalidParameterException("Entity cannot be null.");
    }

    logger.debug("Removing entity: " + entity->getEntityName());

    auto mappingInfo = EntityMapper::getInstance().getMapping(entity->getEntityName());
    if (!mappingInfo) {
        throw MappingException("No mapping information found for entity: " + entity->getEntityName());
    }

    // DELETE 쿼리 생성
    std::string query = "DELETE FROM " + mappingInfo->tableName;
    query += " WHERE " + mappingInfo->idColumnName + " = '" + entity->getId() + "';";

    try {
        connection->executeUpdate(query);
        logger.info("Entity removed successfully.");
    } catch (const QueryExecutionException& e) {
        logger.error(e.what());
        throw;
    }
}

std::shared_ptr<IEntity> Session::find(const std::string& entityName, int id) {
    logger.debug("Finding entity: " + entityName + " with ID: " + std::to_string(id));

    // 1차 캐시 확인
    std::string key = entityName + ":" + std::to_string(id);
    auto it = entityCache.find(key);
    if (it != entityCache.end()) {
        logger.debug("Entity found in cache: " + key);
        return it->second;
    }

    // 2차 캐시 확인
    auto cachedEntity = CacheManager::getInstance().get(key);
    if (cachedEntity) {
        logger.debug("Entity found in second-level cache: " + key);
        entityCache[key] = cachedEntity;
        return cachedEntity;
    }

    auto mappingInfo = EntityMapper::getInstance().getMapping(entityName);
    if (!mappingInfo) {
        throw MappingException("No mapping information found for entity: " + entityName);
    }

    // SELECT 쿼리 생성
    std::string query = "SELECT * FROM " + mappingInfo->tableName;
    query += " WHERE " + mappingInfo->idColumnName + " = '" + std::to_string(id) + "';";

    try {
        auto results = connection->executeQuery(query);
        if (results.empty()) {
            throw EntityNotFoundException("Entity not found: " + entityName + " with ID: " + std::to_string(id));
        }

        // 엔티티 생성 및 필드 설정
        auto entity = mappingInfo->entityConstructor();
        entity->setId(std::to_string(id));
        for (const auto& field : mappingInfo->fields) {
            entity->setFieldValue(field.fieldName, results[0][field.columnName]);
        }

        // 엔티티를 캐시에 저장.
        entityCache[key] = entity;
        CacheManager::getInstance().put(key, entity);

        logger.info("Entity found successfully.");
        return entity;
    } catch (const QueryExecutionException& e) {
        logger.error(e.what());
        throw;
    }
}

std::shared_ptr<IQuery> Session::createQuery(const std::string& queryString) {
    logger.debug("Creating query: " + queryString);
    return std::make_shared<Query>(connection, queryString);
}

std::shared_ptr<ITransaction> Session::beginTransaction() {
    if (isTransactionActive) {
        throw TransactionException("Transaction is already active.");
    }

    try {
        connection->beginTransaction();
        isTransactionActive = true;
        logger.info("Transaction started.");
        return std::make_shared<Transaction>(connection, isTransactionActive);
    } catch (const TransactionException& e) {
        logger.error(e.what());
        throw;
    }
}

std::shared_ptr<IQueryBuilder> Session::createQueryBuilder() {
    logger.debug("Creating QueryBuilder.");
    return std::make_shared<QueryBuilder>(connection);
}

void Session::close() {
    if (connection) {
        if (isTransactionActive) {
            try {
                connection->rollback();
                logger.warn("Transaction was active. Rolled back.");
            } catch (const TransactionException& e) {
                logger.error(e.what());
            }
            isTransactionActive = false;
        }
        connection.reset();
        logger.debug("Session closed.");
    }
}
