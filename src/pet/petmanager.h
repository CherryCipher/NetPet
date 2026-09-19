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
    /**
     * @brief Creates a new PetManager.
     */
    PetManager(
        AnimationManager& animator,
        PetData& data,
        PetStorage& storage,
        ProgressionManager& progression
    );

    /**
     * @brief Initializes NetPet.
     */
    void begin();

    /**
     * @brief Updates NetPet behavior and energy.
     */
    void update();

    /**
     * @brief Reports user activity.
     */
    void activity();

    /**
     * @brief Starts the connection animation.
     */
    void connect();

    /**
     * @brief Adds energy to NetPet.
     *
     * @param amount Energy to add.
     */
    void addEnergy(uint8_t amount);

    /**
     * @brief Returns the current energy level.
     */
    uint8_t getEnergy() const;

    /**
     * @brief Returns the current behavioral state.
     */
    PetState getState() const;

private:
    AnimationManager& animator;
    PetData& data;
    PetStorage& storage;
    ProgressionManager& progression;

    PetState state = PetState::IDLE;

    unsigned long lastActivity = 0;
    unsigned long lastEnergyDecay = 0;

    void updateEnergy();
    void die();
    void resetPet();
    void setState(PetState newState);
};