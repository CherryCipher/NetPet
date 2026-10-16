#pragma once

#include <Arduino.h>
#include <ble/BLEManager.h>

#include "../pet/petdata.h"
#include "../storage/petstorage.h"

/**
 * @brief Provides phone-based wall-clock synchronization over BLE.
 *
 * The phone writes:
 *
 *     PAIRCODE:UNIX_TIMESTAMP
 *
 * Example:
 *
 *     583214:1792123200
 */
class TimeSyncManager {
public:
    TimeSyncManager(BLEManager& ble, PetData& data, PetStorage& storage);

    bool begin();
    void update();

    bool isSynced() const;
    bool isBorn() const;

    uint32_t getPairingCode() const;

    uint64_t now() const;
    uint64_t getLifetimeSeconds() const;

    void reset();

private:
    static constexpr const char* DEVICE_NAME = "NetPet";

    static constexpr const char* SERVICE_UUID =
        "7c6a0001-6d4f-4e65-7450-657400000001";

    static constexpr const char* TIME_UUID =
        "7c6a0002-6d4f-4e65-7450-657400000002";

    BLEManager& ble;
    PetData& data;
    PetStorage& storage;

    uint32_t pairingCode = 0;

    uint64_t syncedUnixTime = 0;
    unsigned long syncedAtMillis = 0;

    bool synced = false;
    bool serviceStarted = false;

    static TimeSyncManager* instance;

    static void handleWrite(
        const String& characteristicUUID,
        const uint8_t* data,
        size_t length
    );

    void processWrite(
        const String& characteristicUUID,
        const uint8_t* data,
        size_t length
    );

    void generatePairingCode();
    bool applyTime(uint64_t unixTime);
};