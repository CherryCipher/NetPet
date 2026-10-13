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
#include "ethernet/ethernetscreen.h"

#include "minigame/minigamescreen.h"

enum class ScreenId : uint8_t {
    PET,
    MENU,
    STATS,
    WIFI,
    BLE,
    MINI_GAME
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
        BLEScreen& bleScreen,
        EthernetScreen& ethernetScreen,
        MiniGameScreen& miniGameScreen
    );

    void begin();
    void update();
    void render();
    void show(ScreenId screen);

    ScreenId getCurrentScreen() const;

private:
    static constexpr uint8_t MENU_ITEM_COUNT = 4;

    Adafruit_SSD1306& display;
    InputManager& input;
    PetManager& pet;
    PetIdle& petIdle;
    AnimationManager& animator;
    PetData& petData;
    ProgressionManager& progression;
    MiniGameScreen& miniGameScreen;

    WiFiScreen& wifiScreen;
    BLEScreen& bleScreen;
    EthernetScreen& ethernetScreen;

    ScreenId currentScreen = ScreenId::PET;
    uint8_t selectedMenuItem = 0;

    void updatePet();
    void updateMenu();
    void updateStats();
    void updateWiFi();
    void updateBLE();
    void updateMiniGame();

    void drawPet();
    void drawMenu();
    void drawStats();
    void drawStatus();
    void selectMenuItem();
};