#pragma once

#include <Arduino.h>
#include "../animation/animationmanager.h"
#include "../assets/animations.h"
#include "../config/gameconfig.h"
#include "../storage/petstorage.h"
#include "../time/timesyncmanager.h"
#include "petdata.h"
#include "progressionmanager.h"

enum class PetState : uint8_t {
    IDLE,
    SLEEP,
    CONNECTING,
    DEAD
};

class PetManager {
public:
    PetManager(
        AnimationManager& animator,
        PetData& data,
        PetStorage& storage,
        ProgressionManager& progression,
        TimeSyncManager& timeSync
    );

    void begin();
    void update();
    void activity();
    void connect();
    void addEnergy(uint8_t amount);

    void pauseEnergyDecay();
    void resumeEnergyDecay();

    void setSleepEnabled(bool enabled);
    void enterPetScreen();

    uint8_t getEnergy() const;
    PetState getState() const;
    bool isEnergyDecayPaused() const;
    bool isSleepEnabled() const;

private:
    AnimationManager& animator;
    PetData& data;
    PetStorage& storage;
    ProgressionManager& progression;
    TimeSyncManager& timeSync;

    PetState state = PetState::IDLE;

    unsigned long lastActivity = 0;

    bool energyDecayPaused = false;
    bool sleepEnabled = true;

    void updateEnergy();
    void die();
    void setState(PetState newState);
};