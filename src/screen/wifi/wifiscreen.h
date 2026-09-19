#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <wifi/WiFiManager.h>

#include "../../input/inputmanager.h"
#include "../../pet/foodmanager.h"

/**
 * @brief Controls the Wi-Fi food hunting screen.
 *
 * WiFiScreen scans nearby access points, filters networks that have already
 * been eaten and allows the player to select and eat available Wi-Fi food.
 *
 * WiFiScreen only draws into the display buffer. ScreenManager remains the
 * sole owner of display.display().
 */
class WiFiScreen {
public:
    WiFiScreen(Adafruit_SSD1306& display, InputManager& input, WiFiManager& wifi, FoodManager& food);

    /**
     * @brief Opens the Wi-Fi screen and performs a new scan.
     */
    void begin();

    /**
     * @brief Updates input and Wi-Fi screen state.
     *
     * @return True when the player wants to return to the menu.
     */
    bool update();

    /**
     * @brief Draws the current Wi-Fi screen into the display buffer.
     */
    void draw();

private:
    static constexpr uint8_t MAX_NETWORKS = 30;
    static constexpr uint8_t MAX_VISIBLE_ITEMS = 4;
    static constexpr unsigned long RESULT_DURATION = 1500;

    enum class State : uint8_t {
        LIST,
        EMPTY,
        ERROR,
        EAT_RESULT
    };

    Adafruit_SSD1306& display;
    InputManager& input;
    WiFiManager& wifi;
    FoodManager& food;

    State state = State::EMPTY;

    int networkIndices[MAX_NETWORKS] = {};
    uint8_t networkCount = 0;
    uint8_t selectedIndex = 0;
    uint8_t scrollOffset = 0;

    String eatenName;
    int32_t eatenRssi = 0;
    EatResult eatResult;
    unsigned long resultStartedAt = 0;

    void scan();
    void rebuildNetworkList();
    void moveUp();
    void moveDown();
    void eatSelected();

    void drawList();
    void drawEmpty();
    void drawError();
    void drawEatResult();

    String getDisplayName(const WiFiNetwork& network) const;
};