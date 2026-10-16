#pragma once

#include <Arduino.h>
#include "../config/gameconfig.h"

/**
 * @brief Persistent NetPet gameplay and lifecycle data.
 */
struct PetData {
    uint32_t xp = 0;
    uint8_t energy = GameConfig::MAX_ENERGY;

    uint32_t wifiEaten = 0;
    uint32_t bleEaten = 0;
    uint32_t ethernetEaten = 0;

    uint32_t miniGameHighScore = 0;

    uint64_t birthTime = 0;
    uint64_t lastSyncTime = 0;
    uint64_t lastEnergyUpdateTime = 0;
};