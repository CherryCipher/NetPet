#include "progressionmanager.h"
#include "../config/gameconfig.h"

#include <esp_timer.h>

ProgressionManager::ProgressionManager(PetData& data) : data(data) {}

void ProgressionManager::begin() {
    resetSession();
}

void ProgressionManager::resetSession() {
    sessionStartedAt = esp_timer_get_time() / 1000ULL;
}

void ProgressionManager::addXp(uint32_t amount) {
    data.xp += amount;
}

uint32_t ProgressionManager::getXp() const {
    return data.xp;
}

uint16_t ProgressionManager::getLevel() const {
    uint32_t requiredXp = 0;

    for (uint16_t level = 1; level < GameConfig::MAX_LEVEL; level++) {
        requiredXp += GameConfig::LEVEL_BASE_XP + ((level - 1) * GameConfig::LEVEL_XP_GROWTH);

        if (data.xp < requiredXp) return level;
    }

    return GameConfig::MAX_LEVEL;
}

uint32_t ProgressionManager::getNextLevelXp() const {
    const uint16_t currentLevel = getLevel();

    if (currentLevel >= GameConfig::MAX_LEVEL) return data.xp;

    uint32_t requiredXp = 0;

    for (uint16_t level = 1; level <= currentLevel; level++) {
        requiredXp += GameConfig::LEVEL_BASE_XP + ((level - 1) * GameConfig::LEVEL_XP_GROWTH);
    }

    return requiredXp;
}


uint64_t ProgressionManager::getUptime() const {
    return (esp_timer_get_time() / 1000ULL) - sessionStartedAt;
}

float ProgressionManager::getUptimeMultiplier() const {
    const uint64_t uptime = getUptime();

    if (uptime >= GameConfig::UPTIME_1M) return 5.0f;
    if (uptime >= GameConfig::UPTIME_1W) return 3.0f;
    if (uptime >= GameConfig::UPTIME_3D) return 2.5f;
    if (uptime >= GameConfig::UPTIME_1D) return 2.25f;
    if (uptime >= GameConfig::UPTIME_12H) return 2.0f;
    if (uptime >= GameConfig::UPTIME_6H) return 1.75f;
    if (uptime >= GameConfig::UPTIME_3H) return 1.5f;
    if (uptime >= GameConfig::UPTIME_1H) return 1.25f;
    if (uptime >= GameConfig::UPTIME_30M) return 1.1f;

    return 1.0f;
}

uint32_t ProgressionManager::calculateRssiXp(int rssi) const {
    if (rssi >= -50) return 50;
    if (rssi >= -60) return 35;
    if (rssi >= -70) return 20;
    if (rssi >= -80) return 10;

    return 5;
}

uint32_t ProgressionManager::calculateFoodXp(int rssi) const {
    return static_cast<uint32_t>(calculateRssiXp(rssi) * getUptimeMultiplier());
}