#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "../animation/animationmanager.h"
#include "../input/inputmanager.h"
#include "../pet/petmanager.h"
#include "../pet/petdata.h"
#include "../pet/progressionmanager.h"
#include "wifi/wifiscreen.h"

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
    ScreenManager(
        Adafruit_SSD1306& display,
        InputManager& input,
        PetManager& pet,
        AnimationManager& animator,
        PetData& petData,
        ProgressionManager& progression,
        WiFiScreen& wifiScreen
    );

    void begin();
    void update();
    void render();
    void show(ScreenId screen);

    ScreenId getCurrentScreen() const;

private:
    static constexpr uint8_t MENU_ITEM_COUNT = 3;

    Adafruit_SSD1306& display;
    InputManager& input;
    PetManager& pet;
    AnimationManager& animator;
    PetData& petData;
    ProgressionManager& progression;
    WiFiScreen& wifiScreen;

    ScreenId currentScreen = ScreenId::PET;
    uint8_t selectedMenuItem = 0;

    void updatePet();
    void updateMenu();
    void updateStats();
    void updateWiFi();
    void updateBLE();

    void drawPet();
    void drawMenu();
    void drawStats();
    void drawBLE();
    void drawStatus();

    void selectMenuItem();
};