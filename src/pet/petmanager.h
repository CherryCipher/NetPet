#pragma once

#include <Arduino.h>

#include "../animation/animationmanager.h"
#include "../assets/animations.h"

/**
 * @brief Represents the current behavioral state of NetPet.
 */
enum class PetState : uint8_t {
    IDLE,
    SLEEP
};

/**
 * @brief Controls the behavioral state of NetPet.
 *
 * PetManager handles state transitions, user activity and inactivity.
 * It selects the appropriate animation for each pet state while leaving
 * animation playback to AnimationManager.
 */
class PetManager {
public:
    /**
     * @brief Creates a new PetManager.
     *
     * @param animator AnimationManager used to display pet animations.
     * @param sleepTimeout Time without activity before NetPet enters sleep.
     */
    PetManager(AnimationManager& animator, unsigned long sleepTimeout = 20000);

    /**
     * @brief Initializes NetPet in the idle state.
     */
    void begin();

    /**
     * @brief Updates the current pet state.
     *
     * This method should be called continuously from loop().
     */
    void update();

    /**
     * @brief Reports user activity to NetPet.
     *
     * Activity resets the inactivity timer and wakes NetPet when sleeping.
     */
    void activity();

    /**
     * @brief Returns the current pet state.
     *
     * @return Current PetState.
     */
    PetState getState() const;

private:
    AnimationManager& animator;

    PetState state = PetState::IDLE;

    unsigned long lastActivity = 0;
    unsigned long sleepTimeout;

    /**
     * @brief Changes the current pet state.
     *
     * @param newState State NetPet should enter.
     */
    void setState(PetState newState);
};