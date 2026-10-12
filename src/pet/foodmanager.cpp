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

    if (preferences.putBool(makeWiFiKey(bssid).c_str(), true) != 1) return EatResult();

    data.wifiEaten++;
    progression.addXp(result.finalXp);
    pet.addEnergy(result.energyGained);

    return result;
}

bool FoodManager::isBLEEaten(const String& address) {
    return preferences.getBool(makeBLEKey(address).c_str(), false);
}

EatResult FoodManager::eatBLE(const String& address, int32_t rssi) {
    EatResult result;
    if (isBLEEaten(address)) return result;

    result.baseXp = progression.calculateRssiXp(rssi);
    result.multiplier = progression.getUptimeMultiplier();
    result.finalXp = progression.calculateFoodXp(rssi);
    result.energyGained = GameConfig::BLE_ENERGY;

    if (preferences.putBool(makeBLEKey(address).c_str(), true) != 1) return EatResult();

    data.bleEaten++;
    progression.addXp(result.finalXp);
    pet.addEnergy(result.energyGained);

    return result;
}

bool FoodManager::isEthernetEaten(const String& gatewayMAC) {
    if (gatewayMAC.isEmpty()) return false;

    return preferences.getBool(makeEthernetKey(gatewayMAC).c_str(), false);
}

EatResult FoodManager::eatEthernet(const String& gatewayMAC) {
    EatResult result;

    if (gatewayMAC.isEmpty()) return result;
    if (isEthernetEaten(gatewayMAC)) return result;

    result.baseXp = GameConfig::ETHERNET_XP;
    result.multiplier = progression.getUptimeMultiplier();
    result.finalXp = static_cast<uint32_t>(result.baseXp * result.multiplier);
    result.energyGained = GameConfig::ETHERNET_ENERGY;

    if (preferences.putBool(makeEthernetKey(gatewayMAC).c_str(), true) != 1) return EatResult();

    data.ethernetEaten++;

    progression.addXp(result.finalXp);
    pet.addEnergy(result.energyGained);

    Serial.print("[FOOD] Ethernet eaten: ");
    Serial.println(gatewayMAC);

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

String FoodManager::makeBLEKey(const String& address) const {
    String key = "b";

    for (size_t i = 0; i < address.length(); i++) {
        const char c = address[i];
        if (c != ':') key += static_cast<char>(tolower(c));
    }

    return key;
}

String FoodManager::makeEthernetKey(const String& gatewayMAC) const {
    String key = "e";

    for (size_t i = 0; i < gatewayMAC.length(); i++) {
        const char c = gatewayMAC[i];
        if (c != ':') key += static_cast<char>(tolower(c));
    }

    return key;
}