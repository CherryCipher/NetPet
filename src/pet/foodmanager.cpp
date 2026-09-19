#include "foodmanager.h"
#include "../config/gameconfig.h"

FoodManager::FoodManager(PetData& data, PetManager& pet, ProgressionManager& progression, PetStorage& storage)
    : data(data), pet(pet), progression(progression), storage(storage) {}

bool FoodManager::begin() {
    return preferences.begin(NAMESPACE, false);
}

bool FoodManager::isWiFiEaten(const String& bssid) {
    return preferences.getBool(makeWiFiKey(bssid).c_str(), false);
}

EatResult FoodManager::eatWiFi(const String& bssid, int32_t rssi) {
    EatResult result;

    if (isWiFiEaten(bssid)) return result;

    result.baseXp = progression.calculateRssiXp(rssi);
    result.multiplier = progression.getUptimeMultiplier();
    result.finalXp = progression.calculateFoodXp(rssi);
    result.energyGained = GameConfig::WIFI_ENERGY;

    preferences.putBool(makeWiFiKey(bssid).c_str(), true);

    data.wifiEaten++;

    progression.addXp(result.finalXp);
    pet.addEnergy(result.energyGained);

    storage.save(data);

    return result;
}

void FoodManager::clear() {
    preferences.clear();
}

String FoodManager::makeWiFiKey(const String& bssid) const {
    String key = "w";

    for (size_t i = 0; i < bssid.length(); i++) {
        const char c = bssid[i];
        if (c != ':') key += static_cast<char>(tolower(c));
    }

    return key;
}