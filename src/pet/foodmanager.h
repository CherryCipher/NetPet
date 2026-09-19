#pragma once

#include <Arduino.h>
#include <Preferences.h>

#include "petdata.h"
#include "petmanager.h"
#include "progressionmanager.h"
#include "../storage/petstorage.h"

/**
 * @brief Result of eating network food.
 */
struct EatResult {
    uint32_t baseXp = 0;
    uint32_t finalXp = 0;
    uint8_t energyGained = 0;
    float multiplier = 1.0f;
};

/**
 * @brief Manages consumed Wi-Fi and BLE food.
 */
class FoodManager {
public:
    /**
     * @brief Creates a new FoodManager.
     */
    FoodManager(PetData& data, PetManager& pet, ProgressionManager& progression, PetStorage& storage);

    /**
     * @brief Initializes persistent food storage.
     */
    bool begin();

    /**
     * @brief Returns whether a Wi-Fi BSSID has already been eaten.
     */
    bool isWiFiEaten(const String& bssid);

    /**
     * @brief Eats a Wi-Fi access point.
     *
     * @param bssid Access point BSSID.
     * @param rssi Current RSSI.
     * @return XP and energy gained.
     */
    EatResult eatWiFi(const String& bssid, int32_t rssi);

    /**
     * @brief Clears all persistent food history.
     */
    void clear();

private:
    static constexpr const char* NAMESPACE = "netpetfood";

    PetData& data;
    PetManager& pet;
    ProgressionManager& progression;
    PetStorage& storage;

    Preferences preferences;

    String makeWiFiKey(const String& bssid) const;
};