#pragma once

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#include "../../animation/animationmanager.h"
#include "../../ethernet/ethernetmanager.h"
#include "../../pet/foodmanager.h"

/**
 * @brief Handles the Ethernet connection, eating and level-up flow.
 */
class EthernetScreen {
public:
    EthernetScreen(
        Adafruit_SSD1306& display,
        EthernetManager& ethernet,
        AnimationManager& animator
    );

    void update();
    void draw();

    void startConnecting();
    void startEating(const EatResult& result, const String& gatewayMAC, bool levelUp, uint16_t level);

    bool isActive() const;
    bool isConnecting() const;

private:
    static constexpr unsigned long RESULT_DURATION = 4000;
    static constexpr unsigned long LEVEL_RESULT_DURATION = 2000;

    enum class State : uint8_t {
        IDLE,
        CONNECTING,
        EATING,
        EAT_RESULT,
        LEVEL_UP,
        LEVEL_UP_RESULT
    };

    Adafruit_SSD1306& display;
    EthernetManager& ethernet;
    AnimationManager& animator;

    State state = State::IDLE;

    EatResult result;
    String gatewayMAC;

    unsigned long stateStartedAt = 0;

    bool levelUpPending = false;
    uint16_t level = 1;

    void finishConnecting();
    void finishEating();
    void startLevelUp();
    void finishFlow();

    void drawEatResult();
    void drawLevelUp();
    void drawLevelUpResult();
};