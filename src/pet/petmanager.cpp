#include "petmanager.h"

PetManager::PetManager(AnimationManager& animator, PetData& data, PetStorage& storage)
    : animator(animator), data(data), storage(storage) {}

void PetManager::begin() {
    lastActivity = millis();
    lastEnergyDecay = millis();
    setState(PetState::IDLE);
}

void PetManager::update() {
    if (state == PetState::DEAD) return;

    updateEnergy();

    if (state == PetState::CONNECTING) {
        if (animator.isFinished()) setState(PetState::IDLE);
        return;
    }

    if (state == PetState::IDLE && millis() - lastActivity >= GameConfig::SLEEP_TIMEOUT) setState(PetState::SLEEP);
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

    const uint16_t newEnergy = static_cast<uint16_t>(data.energy) + amount;
    data.energy = newEnergy > GameConfig::MAX_ENERGY ? GameConfig::MAX_ENERGY : newEnergy;

    storage.save(data);
}

uint8_t PetManager::getEnergy() const {
    return data.energy;
}

PetState PetManager::getState() const {
    return state;
}

void PetManager::updateEnergy() {
    const unsigned long now = millis();

    if (now - lastEnergyDecay < GameConfig::ENERGY_DECAY_INTERVAL) return;

    const unsigned long intervals = (now - lastEnergyDecay) / GameConfig::ENERGY_DECAY_INTERVAL;
    lastEnergyDecay += intervals * GameConfig::ENERGY_DECAY_INTERVAL;

    if (intervals >= data.energy) data.energy = 0;
    else data.energy -= intervals;

    storage.save(data);

    if (data.energy == 0) die();
}

void PetManager::die() {
    state = PetState::DEAD;
    animator.stop();

    storage.clear();

    Serial.println("NetPet died. Save data cleared.");
}

void PetManager::setState(PetState newState) {
    if (state == newState && animator.isPlaying()) return;

    state = newState;

    switch (state) {
        case PetState::IDLE:
            animator.play(ANIMATION_IDLE, true);
            break;

        case PetState::SLEEP:
            animator.play(ANIMATION_SLEEP, true);
            break;

        case PetState::CONNECTING:
            animator.play(ANIMATION_CONNECT, true);
            break;

        case PetState::DEAD:
            animator.stop();
            break;
    }
}