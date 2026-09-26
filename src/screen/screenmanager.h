#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../animation/animationmanager.h"
#include "../input/inputmanager.h"
#include "../pet/petmanager.h"
#include "../pet/petidle.h"
#include "../pet/petdata.h"
#include "../pet/progressionmanager.h"
#include "wifi/wifiscreen.h"
#include "ble/blescreen.h"

enum class ScreenId : uint8_t {
    PET,
    MENU,
    STATS,
    WIFI,
    BLE
};

class ScreenManager {
public:
    ScreenManager(
        Adafruit_SSD1306& display,
        InputManager& input,
        PetManager& pet,
        PetIdle& petIdle,
        AnimationManager& animator,
        PetData& petData,
        ProgressionManager& progression,
        WiFiScreen& wifiScreen,
        BLEScreen& bleScreen
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
    PetIdle& petIdle;
    AnimationManager& animator;
    PetData& petData;
    ProgressionManager& progression;
    WiFiScreen& wifiScreen;
    BLEScreen& bleScreen;

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
    void drawStatus();
    void selectMenuItem();
};