#pragma once

#include <Arduino.h>
#include "petdata.h"

/**
 * @brief Handles NetPet XP, levels and session progression.
 */
class ProgressionManager {
public:
    /**
     * @brief Creates a new ProgressionManager.
     *
     * @param data Persistent pet data.
     */
    explicit ProgressionManager(PetData& data);

    /**
     * @brief Starts a new uptime session.
     */
    void begin();

    /**
     * @brief Adds XP to the pet.
     *
     * @param amount Amount of XP to add.
     */
    void addXp(uint32_t amount);

    /**
     * @brief Returns the current total XP.
     */
    uint32_t getXp() const;

    /**
     * @brief Calculates the current level.
     */
    uint16_t getLevel() const;

    /**
     * @brief Returns the current session uptime in milliseconds.
     */
    unsigned long getUptime() const;

    /**
     * @brief Returns the current uptime XP multiplier.
     */
    float getUptimeMultiplier() const;

    /**
     * @brief Calculates base XP from an RSSI value.
     *
     * @param rssi Signal strength in dBm.
     */
    uint32_t calculateRssiXp(int rssi) const;

    /**
     * @brief Calculates final XP including the uptime multiplier.
     *
     * @param rssi Signal strength in dBm.
     */
    uint32_t calculateFoodXp(int rssi) const;

private:
    PetData& data;
    unsigned long sessionStartedAt = 0;
};