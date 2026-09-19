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

    return true;
}

void PetStorage::save(const PetData& data) {
    preferences.putBool("initialized", true);

    preferences.putULong("xp", data.xp);
    preferences.putUChar("energy", data.energy);
    preferences.putULong("wifiCount", data.wifiEaten);
    preferences.putULong("bleCount", data.bleEaten);
}

void PetStorage::clear() {
    preferences.clear();

    Preferences foodPreferences;

    if (foodPreferences.begin("netpetfood", false)) {
        foodPreferences.clear();
        foodPreferences.end();
    }
}