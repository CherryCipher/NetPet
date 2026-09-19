#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include "../pet/petdata.h"

/**
 * @brief Handles persistent NetPet save data using ESP32 NVS.
 */
class PetStorage {
public:
    /**
     * @brief Initializes persistent storage.
     */
    bool begin();

    /**
     * @brief Loads saved pet data.
     *
     * @param data Destination for loaded data.
     * @return True when existing save data was found.
     */
    bool load(PetData& data);

    /**
     * @brief Saves the current pet data.
     *
     * @param data Pet data to save.
     */
    void save(const PetData& data);

    /**
     * @brief Deletes the complete pet save.
     */
    void clear();

private:
    static constexpr const char* NAMESPACE = "netpet";

    Preferences preferences;
};