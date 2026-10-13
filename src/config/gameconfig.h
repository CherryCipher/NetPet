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
    constexpr uint8_t ETHERNET_ENERGY = 15;

    // Ethernet has no RSSI, so it uses fixed base XP.
    constexpr uint32_t ETHERNET_XP = 100;

    // Level progression.
    constexpr uint16_t MAX_LEVEL = 100;
    constexpr uint32_t LEVEL_BASE_XP = 100;
    constexpr uint32_t LEVEL_XP_GROWTH = 25;

    // Uptime multiplier thresholds.
    constexpr uint64_t UPTIME_30M = 30ULL * 60ULL * 1000ULL;
    constexpr uint64_t UPTIME_1H = 1ULL * 60ULL * 60ULL * 1000ULL;
    constexpr uint64_t UPTIME_3H = 3ULL * 60ULL * 60ULL * 1000ULL;
    constexpr uint64_t UPTIME_6H = 6ULL * 60ULL * 60ULL * 1000ULL;
    constexpr uint64_t UPTIME_12H = 12ULL * 60ULL * 60ULL * 1000ULL;

    constexpr uint64_t UPTIME_1D = 24ULL * 60ULL * 60ULL * 1000ULL;
    constexpr uint64_t UPTIME_3D = 3ULL * 24ULL * 60ULL * 60ULL * 1000ULL;
    constexpr uint64_t UPTIME_1W = 7ULL * 24ULL * 60ULL * 60ULL * 1000ULL;
    constexpr uint64_t UPTIME_1M = 30ULL * 24ULL * 60ULL * 60ULL * 1000ULL;
}