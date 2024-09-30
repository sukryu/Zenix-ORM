#include "CacheManager.h"

CacheManager& CacheManager::getInstance() {
    static CacheManager instance;
    return instance;
}

CacheManager::CacheManager()
    : logger(Logger::getInstance()) {
    logger.debug("CacheManager created.");
}

void CacheManager::put(const std::string& key, const std::shared_ptr<IEntity>& entity) {
    cache[key] = entity;
    logger.debug("Entity cached: " + key);
}

std::shared_ptr<IEntity> CacheManager::get(const std::string& key) {
    auto it = cache.find(key);
    if (it != cache.end()) {
        logger.debug("Entity retrieved from cache: " + key);
        return it->second;
    }
    return nullptr;
}

void CacheManager::remove(const std::string& key) {
    cache.erase(key);
    logger.debug("Entity removed from cache: " + key);
}

void CacheManager::clear() {
    cache.clear();
    logger.debug("Cache cleared.");
}