#include "wifiscreen.h"

WiFiScreen::WiFiScreen(Adafruit_SSD1306& display, InputManager& input, WiFiManager& wifi, FoodManager& food)
    : display(display), input(input), wifi(wifi), food(food) {}

void WiFiScreen::begin() {
    selectedIndex = 0;
    scrollOffset = 0;
    scan();
}

bool WiFiScreen::update() {
    if (state == State::EAT_RESULT) {
        if (millis() - resultStartedAt >= RESULT_DURATION) {
            rebuildNetworkList();
            return true;
        }

        return false;
    }

    if (input.wasPressed(Button::K4)) return true;

    if (state != State::LIST) {
        if (input.wasPressed(Button::K3)) scan();
        return false;
    }

    if (input.wasPressed(Button::K1)) {
        moveUp();
        return false;
    }

    if (input.wasPressed(Button::K2)) {
        moveDown();
        return false;
    }

    if (input.wasPressed(Button::K3)) {
        eatSelected();
        return false;
    }

    return false;
}

void WiFiScreen::draw() {
    switch (state) {
        case State::LIST:
            drawList();
            break;

        case State::EMPTY:
            drawEmpty();
            break;

        case State::ERROR:
            drawError();
            break;

        case State::EAT_RESULT:
            drawEatResult();
            break;
    }
}

void WiFiScreen::scan() {
    networkCount = 0;
    selectedIndex = 0;
    scrollOffset = 0;

    if (!wifi.scanNetworks()) {
        state = State::ERROR;
        return;
    }

    rebuildNetworkList();
}

void WiFiScreen::rebuildNetworkList() {
    networkCount = 0;

    const int scanCount = wifi.getNetworkCount();

    for (int i = 0; i < scanCount && networkCount < MAX_NETWORKS; i++) {
        const WiFiNetwork& network = wifi.getNetwork(i);

        if (food.isWiFiEaten(network.bssid)) continue;

        networkIndices[networkCount] = i;
        networkCount++;
    }

    selectedIndex = 0;
    scrollOffset = 0;

    state = networkCount > 0 ? State::LIST : State::EMPTY;
}

void WiFiScreen::moveUp() {
    if (networkCount == 0) return;

    if (selectedIndex == 0) selectedIndex = networkCount - 1;
    else selectedIndex--;

    if (selectedIndex < scrollOffset) scrollOffset = selectedIndex;

    if (selectedIndex >= scrollOffset + MAX_VISIBLE_ITEMS) {
        scrollOffset = selectedIndex - MAX_VISIBLE_ITEMS + 1;
    }
}

void WiFiScreen::moveDown() {
    if (networkCount == 0) return;

    selectedIndex = (selectedIndex + 1) % networkCount;

    if (selectedIndex < scrollOffset) scrollOffset = selectedIndex;

    if (selectedIndex >= scrollOffset + MAX_VISIBLE_ITEMS) {
        scrollOffset = selectedIndex - MAX_VISIBLE_ITEMS + 1;
    }
}

void WiFiScreen::eatSelected() {
    if (networkCount == 0 || selectedIndex >= networkCount) return;

    const WiFiNetwork& network = wifi.getNetwork(networkIndices[selectedIndex]);

    eatenName = getDisplayName(network);
    eatenRssi = network.rssi;
    eatResult = food.eatWiFi(network.bssid, network.rssi);

    resultStartedAt = millis();
    state = State::EAT_RESULT;
}

void WiFiScreen::drawList() {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("WIFI FOOD ");

    display.print(selectedIndex + 1);
    display.print("/");
    display.print(networkCount);

    for (uint8_t row = 0; row < MAX_VISIBLE_ITEMS; row++) {
        const uint8_t listIndex = scrollOffset + row;

        if (listIndex >= networkCount) break;

        const WiFiNetwork& network = wifi.getNetwork(networkIndices[listIndex]);
        const String name = getDisplayName(network);

        display.setCursor(0, 16 + row * 12);
        display.print(listIndex == selectedIndex ? ">" : " ");

        display.setCursor(8, 16 + row * 12);

        String shortName = name;
        if (shortName.length() > 12) shortName = shortName.substring(0, 12);

        display.print(shortName);

        display.setCursor(92, 16 + row * 12);
        display.print(network.rssi);
    }
}

void WiFiScreen::drawEmpty() {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("WIFI FOOD");

    display.setCursor(0, 22);
    display.print("NO FOOD FOUND");

    display.setCursor(0, 40);
    display.print("K3 RESCAN");

    display.setCursor(0, 54);
    display.print("K4 BACK");
}

void WiFiScreen::drawError() {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("WIFI FOOD");

    display.setCursor(0, 22);
    display.print("SCAN FAILED");

    display.setCursor(0, 40);
    display.print("K3 RETRY");

    display.setCursor(0, 54);
    display.print("K4 BACK");
}

void WiFiScreen::drawEatResult() {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(34, 0);
    display.print("NOM NOM!");

    display.setCursor(0, 14);

    String shortName = eatenName;
    if (shortName.length() > 20) shortName = shortName.substring(0, 20);

    display.print(shortName);

    display.setCursor(0, 26);
    display.print("RSSI: ");
    display.print(eatenRssi);
    display.print(" dBm");

    display.setCursor(0, 38);
    display.print("+");
    display.print(eatResult.energyGained);
    display.print(" ENERGY");

    display.setCursor(0, 50);
    display.print("+");
    display.print(eatResult.finalXp);
    display.print(" XP  x");
    display.print(eatResult.multiplier, 2);
}

String WiFiScreen::getDisplayName(const WiFiNetwork& network) const {
    if (network.ssid.length() > 0) return network.ssid;

    return "<HIDDEN>";
}