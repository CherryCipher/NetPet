#include "inputmanager.h"

InputManager::InputManager(uint8_t k1Pin, uint8_t k2Pin, uint8_t k3Pin, uint8_t k4Pin)
    : pins{k1Pin, k2Pin, k3Pin, k4Pin} {}

void InputManager::begin() {
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        pinMode(pins[i], INPUT_PULLUP);
        currentState[i] = digitalRead(pins[i]) == LOW;
        previousState[i] = currentState[i];
    }
}

void InputManager::update() {
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        previousState[i] = currentState[i];
        currentState[i] = digitalRead(pins[i]) == LOW;
    }
}

bool InputManager::wasPressed(Button button) const {
    uint8_t index = static_cast<uint8_t>(button);
    return currentState[index] && !previousState[index];
}

bool InputManager::anyPressed() const {
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        if (currentState[i] && !previousState[i]) return true;
    }

    return false;
}