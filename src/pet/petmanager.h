#pragma once

#include <Arduino.h>
#include "../animation/animationmanager.h"
#include "../assets/animations.h"

/**
 * @brief Represents the current behavioral state of NetPet.
 */
enum class PetState : uint8_t {
    IDLE,
    SLEEP,
    CONNECTING
};

/**
 * @brief Controls NetPet behavior and animation selection.
 */
class PetManager {
public:
    /**
     * @brief Creates a new PetManager.
     *
     * @param animator Animation manager used by the pet.
     */
    explicit PetManager(AnimationManager& animator);

    /**
     * @brief Initializes NetPet.
     */
    void begin();

    /**
     * @brief Updates NetPet behavior.
     */
    void update();

    /**
     * @brief Reports user activity.
     */
    void activity();

    /**
     * @brief Starts the temporary connection animation.
     */
    void connect();

    /**
     * @brief Returns the current behavioral state.
     */
    PetState getState() const;

private:
    static constexpr unsigned long SLEEP_TIMEOUT = 20000;

    AnimationManager& animator;

    PetState state = PetState::IDLE;
    unsigned long lastActivity = 0;

    void setState(PetState newState);
};