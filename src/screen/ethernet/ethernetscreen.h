#pragma once

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#include "../../ethernet/ethernetmanager.h"
#include "../../pet/foodmanager.h"

/**
 * @brief Displays Ethernet food information and connection status.
 */
class EthernetScreen {
public:
    EthernetScreen(Adafruit_SSD1306& display, EthernetManager& ethernet);

    void showFood(const EatResult& result, const String& gatewayMAC, bool levelUp, uint16_t level);
    void update();
    void draw();

    bool isActive() const;

private:
    static constexpr unsigned long DISPLAY_DURATION = 4000;

    Adafruit_SSD1306& display;
    EthernetManager& ethernet;

    EatResult result;

    String gatewayMAC;

    unsigned long startedAt = 0;

    bool active = false;
    bool levelUp = false;

    uint16_t level = 1;
};