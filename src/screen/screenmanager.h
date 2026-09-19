#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "../animation/animationmanager.h"
#include "../input/inputmanager.h"
#include "../pet/petmanager.h"
#include "../pet/petdata.h"
#include "../pet/progressionmanager.h"

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
     *
     * @param display OLED display used for rendering.
     * @param input Input manager used for button input.
     * @param pet Pet manager controlling NetPet behavior.
     * @param animator Animation manager controlling pet animations.
     * @param petData Persistent NetPet data.
     * @param progression Progression manager controlling XP and levels.
     */
    ScreenManager(
        Adafruit_SSD1306& display,
        InputManager& input,
        PetManager& pet,
        AnimationManager& animator,
        PetData& petData,
        ProgressionManager& progression
    );

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
     *
     * @param screen Screen to activate.
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
    PetData& petData;
    ProgressionManager& progression;

    ScreenId currentScreen = ScreenId::PET;
    uint8_t selectedMenuItem = 0;

    void updatePet();
    void updateMenu();
    void updatePlaceholder();

    void drawPet();
    void drawMenu();
    void drawStats();
    void drawPlaceholder();
    void drawStatus();

    void selectMenuItem();
};