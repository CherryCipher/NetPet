#include "petmanager.h"

PetManager::PetManager(
    AnimationManager& animator,
    PetData& data,
    PetStorage& storage,
    ProgressionManager& progression,
    TimeSyncManager& timeSync
)
    : animator(animator),
      data(data),
      storage(storage),
      progression(progression),
      timeSync(timeSync) {}

void PetManager::begin() {
    lastActivity = millis();

    sleepEnabled = true;
    energyDecayPaused = false;
    state = PetState::IDLE;

    updateEnergy();
}

void PetManager::update() {
    if (state == PetState::DEAD) {
        if (animator.isFinished()) resetPet();
        return;
    }

    updateEnergy();

    if (state == PetState::CONNECTING) {
        if (animator.isFinished()) setState(PetState::IDLE);
        return;
    }

    if (sleepEnabled && state == PetState::IDLE && millis() - lastActivity >= GameConfig::SLEEP_TIMEOUT) {
        setState(PetState::SLEEP);
    }
}

void PetManager::activity() {
    if (state == PetState::DEAD) return;

    lastActivity = millis();

    if (state == PetState::SLEEP) setState(PetState::IDLE);
}

void PetManager::connect() {
    if (state == PetState::DEAD) return;

    lastActivity = millis();
    setState(PetState::CONNECTING);
}

void PetManager::addEnergy(uint8_t amount) {
    if (state == PetState::DEAD) return;

    updateEnergy();

    const uint16_t newEnergy = static_cast<uint16_t>(data.energy) + amount;
    data.energy = newEnergy > GameConfig::MAX_ENERGY ? GameConfig::MAX_ENERGY : newEnergy;

    storage.save(data);
}

void PetManager::pauseEnergyDecay() {
    if (energyDecayPaused) return;

    updateEnergy();
    energyDecayPaused = true;
}

void PetManager::resumeEnergyDecay() {
    if (!energyDecayPaused) return;

    energyDecayPaused = false;

    // Prevent animation time from counting as hunger time.
    if (timeSync.isSynced()) data.lastEnergyUpdateTime = timeSync.now();

    storage.save(data);
}

void PetManager::setSleepEnabled(bool enabled) {
    if (sleepEnabled == enabled) return;

    sleepEnabled = enabled;
    lastActivity = millis();

    if (!sleepEnabled && state == PetState::SLEEP) setState(PetState::IDLE);
}

void PetManager::enterPetScreen() {
    if (state == PetState::DEAD) return;

    sleepEnabled = true;
    lastActivity = millis();
    state = PetState::IDLE;
}

uint8_t PetManager::getEnergy() const {
    return data.energy;
}

PetState PetManager::getState() const {
    return state;
}

bool PetManager::isEnergyDecayPaused() const {
    return energyDecayPaused;
}

bool PetManager::isSleepEnabled() const {
    return sleepEnabled;
}

void PetManager::updateEnergy() {
    if (energyDecayPaused || !timeSync.isSynced()) return;

    const uint64_t currentTime = timeSync.now();

    if (data.lastEnergyUpdateTime == 0) {
        data.lastEnergyUpdateTime = currentTime;
        storage.save(data);
        return;
    }

    if (currentTime <= data.lastEnergyUpdateTime) return;

    const uint64_t elapsed = currentTime - data.lastEnergyUpdateTime;
    const uint64_t intervals = elapsed / GameConfig::ENERGY_DECAY_INTERVAL_SECONDS;

    if (intervals == 0) return;

    data.lastEnergyUpdateTime += intervals * GameConfig::ENERGY_DECAY_INTERVAL_SECONDS;

    if (intervals >= data.energy) data.energy = 0;
    else data.energy -= static_cast<uint8_t>(intervals);

    storage.save(data);

    Serial.print("[ENERGY] -");
    Serial.print(static_cast<unsigned long long>(intervals));
    Serial.print(" | Energy: ");
    Serial.println(data.energy);

    if (data.energy == 0) die();
}

void PetManager::die() {
    Serial.println("NetPet died.");

    energyDecayPaused = false;
    sleepEnabled = false;

    setState(PetState::DEAD);
}

void PetManager::resetPet() {
    Serial.println("Creating new NetPet.");

    storage.clear();

    data = PetData();
    storage.save(data);

    progression.resetSession();
    timeSync.reset();

    lastActivity = millis();

    energyDecayPaused = false;
    sleepEnabled = true;

    setState(PetState::IDLE);
}

void PetManager::setState(PetState newState) {
    if (state == newState) return;

    state = newState;

    switch (state) {
        case PetState::IDLE:
        case PetState::SLEEP:
            break;

        case PetState::CONNECTING:
            break;

        case PetState::DEAD:
            animator.play(ANIMATION_DEATH, true);
            break;
    }
}