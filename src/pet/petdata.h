#pragma once

#include <Arduino.h>
#include "../config/gameconfig.h"

/**
 * @brief Persistent NetPet gameplay data.
 *
 * Values in this structure survive resets and power cycles.
 * Session uptime is intentionally not stored here.
 */
struct PetData {
    uint32_t xp = 0;
    uint8_t energy = GameConfig::MAX_ENERGY;

    uint32_t wifiEaten = 0;
    uint32_t bleEaten = 0;
};