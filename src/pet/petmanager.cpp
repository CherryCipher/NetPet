#include "petmanager.h"

PetManager::PetManager(AnimationManager& animator) : animator(animator) {}

void PetManager::begin() {
    lastActivity = millis();
    setState(PetState::IDLE);
}

void PetManager::update() {
    if (state == PetState::CONNECTING) {
        if (animator.isFinished()) setState(PetState::IDLE);
        return;
    }

    if (state == PetState::IDLE && millis() - lastActivity >= SLEEP_TIMEOUT) setState(PetState::SLEEP);
}

void PetManager::activity() {
    lastActivity = millis();

    if (state == PetState::SLEEP) setState(PetState::IDLE);
}

void PetManager::connect() {
    lastActivity = millis();
    setState(PetState::CONNECTING);
}

PetState PetManager::getState() const {
    return state;
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
    }
}