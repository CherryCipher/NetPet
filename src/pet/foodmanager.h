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
    FoodManager(PetData& data, PetManager& pet, ProgressionManager& progression, PetStorage& storage);

    bool begin();

    bool isWiFiEaten(const String& bssid);
    EatResult eatWiFi(const String& bssid, int32_t rssi);

    bool isBLEEaten(const String& address);
    EatResult eatBLE(const String& address, int32_t rssi);

    void clear();

private:
    static constexpr const char* NAMESPACE = "netpetfood";

    PetData& data;
    PetManager& pet;
    ProgressionManager& progression;
    PetStorage& storage;
    Preferences preferences;

    String makeWiFiKey(const String& bssid) const;
    String makeBLEKey(const String& address) const;
};