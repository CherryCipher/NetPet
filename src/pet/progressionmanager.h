#pragma once

#include <Arduino.h>
#include "petdata.h"

class TimeSyncManager;

/**
 * @brief Handles NetPet XP, levels and lifetime progression.
 */
class ProgressionManager {
public:
    explicit ProgressionManager(PetData& data);

    void begin();
    void setTimeSyncManager(TimeSyncManager& timeSync);
    void resetSession();

    void addXp(uint32_t amount);

    uint32_t getXp() const;
    uint16_t getLevel() const;
    uint32_t getNextLevelXp() const;

    uint64_t getUptime() const;
    float getUptimeMultiplier() const;

    uint32_t calculateRssiXp(int rssi) const;
    uint32_t calculateFoodXp(int rssi) const;

private:
    PetData& data;
    TimeSyncManager* timeSync = nullptr;
};