#pragma once

#include <Arduino.h>

/**
 * @brief Contains all configurable NetPet gameplay values.
 */
namespace GameConfig {
    constexpr uint8_t MAX_ENERGY = 100;

    // Development value. Change this later for final gameplay balancing.
    constexpr unsigned long ENERGY_DECAY_INTERVAL = 500;

    constexpr unsigned long SLEEP_TIMEOUT = 20000;

    constexpr uint8_t WIFI_ENERGY = 10;
    constexpr uint8_t BLE_ENERGY = 5;

    constexpr uint32_t LEVEL_BASE_XP = 100;

    constexpr unsigned long UPTIME_1 = 1UL * 60UL * 60UL * 1000UL;
    constexpr unsigned long UPTIME_3 = 3UL * 60UL * 60UL * 1000UL;
    constexpr unsigned long UPTIME_6 = 6UL * 60UL * 60UL * 1000UL;
    constexpr unsigned long UPTIME_12 = 12UL * 60UL * 60UL * 1000UL;
    constexpr unsigned long UPTIME_24 = 24UL * 60UL * 60UL * 1000UL;
}