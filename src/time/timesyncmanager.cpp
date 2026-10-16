#include "timesyncmanager.h"

TimeSyncManager* TimeSyncManager::instance = nullptr;

TimeSyncManager::TimeSyncManager(BLEManager& ble, PetData& data, PetStorage& storage)
    : ble(ble), data(data), storage(storage) {}

bool TimeSyncManager::begin() {
    instance = this;

    synced = false;
    serviceStarted = false;
    syncedUnixTime = 0;
    syncedAtMillis = 0;

    generatePairingCode();

    if (!ble.createServer(SERVICE_UUID)) {
        Serial.println("[TIME] Failed to create BLE service.");
        return false;
    }

    if (!ble.addServerCharacteristic(
        TIME_UUID,
        NIMBLE_PROPERTY::WRITE,
        handleWrite
    )) {
        Serial.println("[TIME] Failed to create time characteristic.");
        return false;
    }

    if (!ble.startServer()) {
        Serial.println("[TIME] Failed to start BLE server.");
        return false;
    }

    if (!ble.startAdvertising(DEVICE_NAME, SERVICE_UUID)) {
        Serial.println("[TIME] Failed to start advertising.");
        return false;
    }

    serviceStarted = true;

    Serial.print("[TIME] Waiting for phone. Pair code: ");
    Serial.println(pairingCode);

    return true;
}

void TimeSyncManager::update() {
    if (!serviceStarted) return;
}

bool TimeSyncManager::isSynced() const {
    return synced;
}

bool TimeSyncManager::isBorn() const {
    return data.birthTime != 0;
}

uint32_t TimeSyncManager::getPairingCode() const {
    return pairingCode;
}

uint64_t TimeSyncManager::now() const {
    if (!synced) return 0;

    return syncedUnixTime + ((millis() - syncedAtMillis) / 1000ULL);
}

uint64_t TimeSyncManager::getLifetimeSeconds() const {
    if (!synced || data.birthTime == 0) return 0;

    const uint64_t currentTime = now();

    if (currentTime <= data.birthTime) return 0;

    return currentTime - data.birthTime;
}

void TimeSyncManager::reset() {
    synced = false;
    syncedUnixTime = 0;
    syncedAtMillis = 0;
}

void TimeSyncManager::handleWrite(
    const String& characteristicUUID,
    const uint8_t* data,
    size_t length
) {
    if (instance == nullptr) return;

    instance->processWrite(characteristicUUID, data, length);
}

void TimeSyncManager::processWrite(
    const String& characteristicUUID,
    const uint8_t* rawData,
    size_t length
) {
    if (characteristicUUID != TIME_UUID) return;
    if (rawData == nullptr || length == 0) return;

    String value;

    for (size_t i = 0; i < length; i++) value += static_cast<char>(rawData[i]);

    value.trim();

    const int separator = value.indexOf(':');

    if (separator <= 0) {
        Serial.println("[TIME] Invalid sync format.");
        return;
    }

    const String codeText = value.substring(0, separator);
    const String timeText = value.substring(separator + 1);

    const uint32_t receivedCode = static_cast<uint32_t>(strtoul(codeText.c_str(), nullptr, 10));
    const uint64_t unixTime = strtoull(timeText.c_str(), nullptr, 10);

    if (receivedCode != pairingCode) {
        Serial.println("[TIME] Incorrect pairing code.");
        return;
    }

    if (!applyTime(unixTime)) {
        Serial.println("[TIME] Invalid timestamp.");
        return;
    }

    Serial.println("[TIME] Phone time accepted.");
}

void TimeSyncManager::generatePairingCode() {
    randomSeed(
        static_cast<uint32_t>(esp_random()) ^
        static_cast<uint32_t>(micros())
    );

    pairingCode = random(100000, 1000000);
}

bool TimeSyncManager::applyTime(uint64_t unixTime) {
    // Reject obviously invalid Unix timestamps.
    if (unixTime < 1700000000ULL) return false;

    syncedUnixTime = unixTime;
    syncedAtMillis = millis();
    synced = true;

    if (data.birthTime == 0) {
        data.birthTime = unixTime;
        data.lastSyncTime = unixTime;
        data.lastEnergyUpdateTime = unixTime;

        storage.save(data);

        Serial.print("[TIME] NetPet born at ");
        Serial.println(static_cast<unsigned long long>(unixTime));

        return true;
    }

    if (unixTime < data.lastSyncTime) {
        Serial.println("[TIME] Phone clock is older than previous sync.");

        data.lastSyncTime = unixTime;
        data.lastEnergyUpdateTime = unixTime;
        storage.save(data);

        return true;
    }

    data.lastSyncTime = unixTime;

    storage.save(data);

    Serial.print("[TIME] Time synchronized: ");
    Serial.println(static_cast<unsigned long long>(unixTime));

    return true;
}