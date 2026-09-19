#include "progressionmanager.h"
#include "../config/gameconfig.h"

ProgressionManager::ProgressionManager(PetData& data) : data(data) {}

void ProgressionManager::begin() {
    sessionStartedAt = millis();
}

void ProgressionManager::addXp(uint32_t amount) {
    data.xp += amount;
}

uint32_t ProgressionManager::getXp() const {
    return data.xp;
}

uint16_t ProgressionManager::getLevel() const {
    return 1 + data.xp / GameConfig::LEVEL_BASE_XP;
}

unsigned long ProgressionManager::getUptime() const {
    return millis() - sessionStartedAt;
}

float ProgressionManager::getUptimeMultiplier() const {
    const unsigned long uptime = getUptime();

    if (uptime >= GameConfig::UPTIME_24) return 2.0f;
    if (uptime >= GameConfig::UPTIME_12) return 1.75f;
    if (uptime >= GameConfig::UPTIME_6) return 1.5f;
    if (uptime >= GameConfig::UPTIME_3) return 1.25f;
    if (uptime >= GameConfig::UPTIME_1) return 1.1f;

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