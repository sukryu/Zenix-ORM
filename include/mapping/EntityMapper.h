#ifndef ENTITY_MAPPER_H
#define ENTITY_MAPPER_H

#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include "IEntity.h"
#include "include/utils/logger/Logger.h"

struct FieldMapping {
    std::string fieldName;
    std::string columnName;
};

struct Relationship {
    std::string relationshipName; // 관계 이름 (필드 이름)
    std::string type; // "OneToOne", "OneToMany", "ManyToOne", "ManyToMany"
    std::string targetEntity;
    std::string mappedBy; // 상태 엔티티에서 이 엔티티를 참조하는 필드
    std::string joinColumn; // 외래 키 컬럼.
};

struct EntityMapping {
    std::string entityName;
    std::string tableName;
    std::string idColumnName;
    std::vector<FieldMapping> fields;
    std::vector<Relationship> relationships;
    std::function<std::shared_ptr<IEntity>()> entityConstructor;
};

class EntityMapper {
public:
    static EntityMapper& getInstance();

    void registerEntity(const EntityMapping& mapping);
    std::shared_ptr<EntityMapping> getMapping(const std::string& entityName);
    std::shared_ptr<EntityMapping> getMappingByTableName(const std::string& tableName);

private:
    EntityMapper();
    EntityMapper(const EntityMapper&) = delete;
    EntityMapper& operator=(const EntityMapper&) = delete;

    std::unordered_map<std::string, EntityMapping> mappings;
    Logger& logger;
};

#endif // ENTITY_MAPPER_H