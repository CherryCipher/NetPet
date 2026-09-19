#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../animation/animationmanager.h"
#include "../input/inputmanager.h"
#include "../pet/petmanager.h"

/**
 * @brief Identifies the available NetPet screens.
 */
enum class ScreenId : uint8_t {
    PET,
    MENU,
    STATS,
    WIFI,
    BLE
};

/**
 * @brief Controls NetPet screen navigation and rendering.
 *
 * ScreenManager is the only component allowed to send the display buffer
 * to the physical OLED.
 */
class ScreenManager {
public:
    /**
     * @brief Creates a new ScreenManager.
     */
    ScreenManager(Adafruit_SSD1306& display, InputManager& input, PetManager& pet, AnimationManager& animator);

    /**
     * @brief Initializes the screen system.
     */
    void begin();

    /**
     * @brief Handles input and screen navigation.
     */
    void update();

    /**
     * @brief Renders the active screen.
     */
    void render();

    /**
     * @brief Activates a screen.
     */
    void show(ScreenId screen);

    /**
     * @brief Returns the active screen.
     */
    ScreenId getCurrentScreen() const;

private:
    static constexpr uint8_t MENU_ITEM_COUNT = 3;

    Adafruit_SSD1306& display;
    InputManager& input;
    PetManager& pet;
    AnimationManager& animator;

    ScreenId currentScreen = ScreenId::PET;
    uint8_t selectedMenuItem = 0;

    void updatePet();
    void updateMenu();
    void updatePlaceholder();

    void drawPet();
    void drawMenu();
    void drawPlaceholder();
    void drawStatus();

    void selectMenuItem();
};