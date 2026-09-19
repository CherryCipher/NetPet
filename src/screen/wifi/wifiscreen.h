#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <wifi/WiFiManager.h>

#include "../../animation/animationmanager.h"
#include "../../assets/animations.h"
#include "../../input/inputmanager.h"
#include "../../pet/foodmanager.h"
#include "../../pet/petmanager.h"
#include "../../pet/progressionmanager.h"

/**
 * @brief Controls the Wi-Fi food hunting and eating flow.
 */
class WiFiScreen {
public:
    WiFiScreen(
        Adafruit_SSD1306& display,
        InputManager& input,
        WiFiManager& wifi,
        FoodManager& food,
        PetManager& pet,
        ProgressionManager& progression,
        AnimationManager& animator
    );

    void begin();
    bool update();
    void draw();

private:
    static constexpr uint8_t MAX_NETWORKS = 30;
    static constexpr uint8_t MAX_VISIBLE_ITEMS = 4;

    static constexpr unsigned long EAT_RESULT_DURATION = 1800;
    static constexpr unsigned long LEVEL_RESULT_DURATION = 1800;

    enum class State : uint8_t {
        LIST,
        EMPTY,
        ERROR,
        EATING,
        EAT_RESULT,
        LEVEL_UP,
        LEVEL_UP_RESULT
    };

    Adafruit_SSD1306& display;
    InputManager& input;
    WiFiManager& wifi;
    FoodManager& food;
    PetManager& pet;
    ProgressionManager& progression;
    AnimationManager& animator;

    State state = State::EMPTY;

    int networkIndices[MAX_NETWORKS] = {};
    uint8_t networkCount = 0;
    uint8_t selectedIndex = 0;
    uint8_t scrollOffset = 0;

    String eatenName;
    EatResult eatResult;

    uint16_t previousLevel = 1;
    uint16_t newLevel = 1;

    unsigned long stateStartedAt = 0;

    void scan();
    void rebuildNetworkList();
    void moveUp();
    void moveDown();

    void startEating();
    void finishEating();
    void finishFlow();

    void drawList();
    void drawEmpty();
    void drawError();
    void drawEating();
    void drawEatResult();
    void drawLevelUp();
    void drawLevelUpResult();

    String getDisplayName(const WiFiNetwork& network) const;
};