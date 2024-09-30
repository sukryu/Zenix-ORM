#ifndef CACHE_MANAGER_H
#define CACHE_MANAGER_H

#include <string>
#include <memory>
#include <unordered_map>
#include "../mapping/IEntity.h"
#include "../utils/logger/Logger.h"

class CacheManager {
public:
    static CacheManager& getInstance();

    void put(const std::string& key, const std::shared_ptr<IEntity>& entity);
    std::shared_ptr<IEntity> get(const std::string& key);
    void remove(const std::string& key);
    void clear();

private:
    CacheManager();
    CacheManager(const CacheManager&) = delete;
    CacheManager& operator=(const CacheManager&) = delete;

    std::unordered_map<std::string, std::shared_ptr<IEntity>> cache;
    Logger& logger;
};

#endif // CACHE_MANAGER_H