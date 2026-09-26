#pragma once

#include <Arduino.h>

/**
 * @brief Contains all configurable NetPet gameplay values.
 */
namespace GameConfig {
    constexpr uint8_t MAX_ENERGY = 100;
    constexpr uint8_t SICK_ENERGY = 25;

    // Energy decreases by 1 every 15 minutes.
    constexpr unsigned long ENERGY_DECAY_INTERVAL = 15UL * 60UL * 1000UL;

    // Pet goes to sleep after 5 minutes without activity.
    constexpr unsigned long SLEEP_TIMEOUT = 5UL * 60UL * 1000UL;

    // Energy gained from food.
    constexpr uint8_t WIFI_ENERGY = 8;
    constexpr uint8_t BLE_ENERGY = 5;

    // Level progression.
    constexpr uint16_t MAX_LEVEL = 100;
    constexpr uint32_t LEVEL_BASE_XP = 100;
    constexpr uint32_t LEVEL_XP_GROWTH = 25;

    // Uptime multiplier thresholds.
    constexpr unsigned long UPTIME_1 = 1UL * 60UL * 60UL * 1000UL;
    constexpr unsigned long UPTIME_3 = 3UL * 60UL * 60UL * 1000UL;
    constexpr unsigned long UPTIME_6 = 6UL * 60UL * 60UL * 1000UL;
    constexpr unsigned long UPTIME_12 = 12UL * 60UL * 60UL * 1000UL;
    constexpr unsigned long UPTIME_24 = 24UL * 60UL * 60UL * 1000UL;
}