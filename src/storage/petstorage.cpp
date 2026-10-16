#include "petstorage.h"

bool PetStorage::begin() {
    return preferences.begin(NAMESPACE, false);
}

bool PetStorage::load(PetData& data) {
    if (!preferences.isKey("initialized")) return false;

    data.xp = preferences.getULong("xp", 0);
    data.energy = preferences.getUChar("energy", GameConfig::MAX_ENERGY);

    data.wifiEaten = preferences.getULong("wifiCount", 0);
    data.bleEaten = preferences.getULong("bleCount", 0);
    data.ethernetEaten = preferences.getULong("ethCount", 0);

    data.miniGameHighScore = preferences.getULong("gameHigh", 0);

    data.birthTime = preferences.getULong64("birthTime", 0);
    data.lastSyncTime = preferences.getULong64("lastSync", 0);
    data.lastEnergyUpdateTime = preferences.getULong64("energyTime", 0);

    return true;
}

void PetStorage::save(const PetData& data) {
    preferences.putBool("initialized", true);

    preferences.putULong("xp", data.xp);
    preferences.putUChar("energy", data.energy);

    preferences.putULong("wifiCount", data.wifiEaten);
    preferences.putULong("bleCount", data.bleEaten);
    preferences.putULong("ethCount", data.ethernetEaten);

    preferences.putULong("gameHigh", data.miniGameHighScore);

    preferences.putULong64("birthTime", data.birthTime);
    preferences.putULong64("lastSync", data.lastSyncTime);
    preferences.putULong64("energyTime", data.lastEnergyUpdateTime);
}

void PetStorage::clear() {
    preferences.clear();

    Preferences foodPreferences;

    if (foodPreferences.begin("netpetfood", false)) {
        foodPreferences.clear();
        foodPreferences.end();
    }
}