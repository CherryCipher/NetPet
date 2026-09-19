#pragma once

#include <Arduino.h>

#include "../animation/animationmanager.h"
#include "../assets/animations.h"
#include "../config/gameconfig.h"
#include "../storage/petstorage.h"
#include "petdata.h"
#include "progressionmanager.h"

/**
 * @brief Represents the current behavioral state of NetPet.
 */
enum class PetState : uint8_t {
    IDLE,
    SLEEP,
    CONNECTING,
    DEAD
};

/**
 * @brief Controls NetPet behavior, energy and life cycle.
 */
class PetManager {
public:
    PetManager(AnimationManager& animator, PetData& data, PetStorage& storage, ProgressionManager& progression);

    void begin();
    void update();
    void activity();
    void connect();

    void addEnergy(uint8_t amount);

    void pauseEnergyDecay();
    void resumeEnergyDecay();

    uint8_t getEnergy() const;
    PetState getState() const;
    bool isEnergyDecayPaused() const;

private:
    AnimationManager& animator;
    PetData& data;
    PetStorage& storage;
    ProgressionManager& progression;

    PetState state = PetState::IDLE;

    unsigned long lastActivity = 0;
    unsigned long lastEnergyDecay = 0;

    bool energyDecayPaused = false;

    void updateEnergy();
    void die();
    void resetPet();
    void setState(PetState newState);
};