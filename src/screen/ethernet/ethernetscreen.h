#pragma once

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#include "../../animation/animationmanager.h"
#include "../../ethernet/ethernetmanager.h"
#include "../../pet/foodmanager.h"

/**
 * @brief Handles the Ethernet connection, eating animation and result screen.
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
    static constexpr unsigned long DISPLAY_DURATION = 4000;

    enum class State : uint8_t {
        IDLE,
        CONNECTING,
        EATING,
        RESULT
    };

    Adafruit_SSD1306& display;
    EthernetManager& ethernet;
    AnimationManager& animator;

    State state = State::IDLE;

    EatResult result;
    String gatewayMAC;

    unsigned long startedAt = 0;

    bool levelUp = false;
    uint16_t level = 1;

    void finishConnecting();
    void finishEating();
    void finishResult();

    void drawResult();
};