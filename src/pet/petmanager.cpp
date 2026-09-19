#include "petmanager.h"

PetManager::PetManager(AnimationManager& animator, unsigned long sleepTimeout)
    : animator(animator), sleepTimeout(sleepTimeout) {}

void PetManager::begin() {
    lastActivity = millis();
    setState(PetState::IDLE);
}

void PetManager::update() {
    if (state == PetState::CONNECTING) {
        if (!animator.isPlaying()) setState(PetState::IDLE);
        return;
    }

    if (state == PetState::IDLE && millis() - lastActivity >= sleepTimeout) setState(PetState::SLEEP);
}

void PetManager::activity() {
    lastActivity = millis();

    if (state == PetState::SLEEP) setState(PetState::IDLE);
}

PetState PetManager::getState() const {
    return state;
}

void PetManager::setState(PetState newState) {
    state = newState;

    switch (state) {
        case PetState::IDLE:
            animator.play(ANIMATION_IDLE, true);
            break;

        case PetState::SLEEP:
            animator.play(ANIMATION_SLEEP, true);
            break;
        
        case PetState::CONNECTING:
            animator.play(ANIMATION_CONNECT, false);
            break;
    }
}

void PetManager::connect() {
    lastActivity = millis();
    setState(PetState::CONNECTING);
}