#include "include/mapping/EntityMapper.h"
#include "include/utils/ORMException/MappingException/MappingException.h"

EntityMapper& EntityMapper::getInstance() {
    static EntityMapper instance;
    return instance;
}

EntityMapper::EntityMapper()
    : logger(Logger::getInstance()) {
    logger.debug("EntityMapper created.");
}

void EntityMapper::registerEntity(const EntityMapping& mapping) {
    mappings[mapping.entityName] = mapping;
    logger.info("Entity registered: " + mapping.entityName);
}

std::shared_ptr<EntityMapping> EntityMapper::getMapping(const std::string& entityName) {
    auto it = mappings.find(entityName);
    if (it != mappings.end()) {
        return std::make_shared<EntityMapping>(it->second);
    } else {
        logger.error("No mapping found for entity: " + entityName);
        return nullptr;
    }
}

std::shared_ptr<EntityMapping> EntityMapper::getMappingByTableName(const std::string& tableName) {
    for (const auto& pair : mappings) {
        if (pair.second.tableName == tableName) {
            return std::make_shared<EntityMapping>(pair.second);
        }
    }
    logger.error("No mapping found for table: " + tableName);
    return nullptr;
}