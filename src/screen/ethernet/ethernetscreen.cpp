#include "ethernetscreen.h"

EthernetScreen::EthernetScreen(Adafruit_SSD1306& display, EthernetManager& ethernet)
    : display(display), ethernet(ethernet) {}

void EthernetScreen::showFood(const EatResult& result, const String& gatewayMAC, bool levelUp, uint16_t level) {
    this->result = result;
    this->gatewayMAC = gatewayMAC;
    this->levelUp = levelUp;
    this->level = level;

    startedAt = millis();
    active = true;
}

void EthernetScreen::update() {
    if (!active) return;

    if (millis() - startedAt >= DISPLAY_DURATION) active = false;
}

void EthernetScreen::draw() {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("ETHERNET EATEN!");

    display.drawFastHLine(0, 11, 128, SSD1306_WHITE);

    display.setCursor(0, 18);
    display.print(gatewayMAC);

    display.setCursor(0, 32);
    display.print("+");
    display.print(result.energyGained);
    display.print(" ENERGY");

    display.setCursor(0, 43);
    display.print("+");
    display.print(result.finalXp);
    display.print(" XP");

    display.setCursor(75, 43);
    display.print("x");
    display.print(result.multiplier, 1);

    display.setCursor(0, 55);

    if (levelUp) {
        display.print("LEVEL UP! LV ");
        display.print(level);
    } else {
        display.print("GATEWAY DISCOVERED");
    }
}

bool EthernetScreen::isActive() const {
    return active;
}