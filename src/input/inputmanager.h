#pragma once

#include <Arduino.h>

/**
 * @brief Identifies the physical NetPet buttons.
 */
enum class Button : uint8_t {
    K1,
    K2,
    K3,
    K4,
    COUNT
};

/**
 * @brief Handles NetPet button input and press detection.
 *
 * InputManager detects new button presses instead of repeatedly triggering
 * while a button remains held down.
 */
class InputManager {
public:
    /**
     * @brief Creates a new InputManager.
     *
     * @param k1Pin GPIO pin connected to K1.
     * @param k2Pin GPIO pin connected to K2.
     * @param k3Pin GPIO pin connected to K3.
     * @param k4Pin GPIO pin connected to K4.
     */
    InputManager(uint8_t k1Pin, uint8_t k2Pin, uint8_t k3Pin, uint8_t k4Pin);

    /**
     * @brief Initializes all button GPIO pins.
     */
    void begin();

    /**
     * @brief Updates the state of all buttons.
     *
     * This method should be called continuously from loop().
     */
    void update();

    /**
     * @brief Checks whether a button was newly pressed.
     *
     * @param button Button to check.
     * @return true once when the button transitions from released to pressed.
     */
    bool wasPressed(Button button) const;

    /**
     * @brief Checks whether any button was newly pressed.
     *
     * @return true if any button has just been pressed.
     */
    bool anyPressed() const;

private:
    static constexpr uint8_t BUTTON_COUNT = static_cast<uint8_t>(Button::COUNT);

    uint8_t pins[BUTTON_COUNT];
    bool currentState[BUTTON_COUNT] = {};
    bool previousState[BUTTON_COUNT] = {};
};