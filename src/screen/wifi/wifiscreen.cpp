#include "wifiscreen.h"

WiFiScreen::WiFiScreen(
    Adafruit_SSD1306& display,
    InputManager& input,
    WiFiManager& wifi,
    FoodManager& food,
    PetManager& pet,
    ProgressionManager& progression,
    AnimationManager& animator
)
    : display(display),
      input(input),
      wifi(wifi),
      food(food),
      pet(pet),
      progression(progression),
      animator(animator) {}

void WiFiScreen::begin() {
    selectedIndex = 0;
    scrollOffset = 0;
    scan();
}

bool WiFiScreen::update() {
    switch (state) {
        case State::EATING:
            if (animator.isFinished()) finishEating();
            return false;

        case State::EAT_RESULT:
            if (millis() - stateStartedAt >= EAT_RESULT_DURATION) {
                if (newLevel > previousLevel) startLevelUp();
                else finishFlow();
            }
            return false;

        case State::LEVEL_UP:
            if (animator.isFinished()) {
                stateStartedAt = millis();
                state = State::LEVEL_UP_RESULT;
            }
            return false;

        case State::LEVEL_UP_RESULT:
            if (millis() - stateStartedAt >= LEVEL_RESULT_DURATION) finishFlow();
            return false;

        default:
            break;
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
        startEating();
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

        case State::EATING:
            drawEating();
            break;

        case State::EAT_RESULT:
            drawEatResult();
            break;

        case State::LEVEL_UP:
            drawLevelUp();
            break;

        case State::LEVEL_UP_RESULT:
            drawLevelUpResult();
            break;
    }
}

void WiFiScreen::scan() {
    networkCount = 0;
    selectedIndex = 0;
    scrollOffset = 0;

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(4, 20);
    display.print("SEARCHING FOR FOOD");

    display.setCursor(32, 38);
    display.print("PLEASE WAIT");

    display.display();

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

void WiFiScreen::startEating() {
    if (networkCount == 0 || selectedIndex >= networkCount) return;

    const WiFiNetwork& network = wifi.getNetwork(networkIndices[selectedIndex]);

    eatenName = getDisplayName(network);

    previousLevel = progression.getLevel();

    pet.pauseEnergyDecay();

    eatResult = food.eatWiFi(network.bssid, network.rssi);

    newLevel = progression.getLevel();

    Serial.print("WiFi eaten: ");
    Serial.print(eatenName);
    Serial.print(" | XP +");
    Serial.print(eatResult.finalXp);
    Serial.print(" | Level ");
    Serial.print(previousLevel);
    Serial.print(" -> ");
    Serial.println(newLevel);

    animator.play(ANIMATION_EAT, true);

    state = State::EATING;
}

void WiFiScreen::finishEating() {
    stateStartedAt = millis();
    state = State::EAT_RESULT;
}

void WiFiScreen::startLevelUp() {
    Serial.print("Level up! ");
    Serial.print(previousLevel);
    Serial.print(" -> ");
    Serial.println(newLevel);

    animator.play(ANIMATION_LEVEL_UP, true);

    state = State::LEVEL_UP;
}

void WiFiScreen::finishFlow() {
    pet.resumeEnergyDecay();

    rebuildNetworkList();
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

        String name = getDisplayName(network);
        if (name.length() > 12) name = name.substring(0, 12);

        display.setCursor(0, 16 + row * 12);
        display.print(listIndex == selectedIndex ? ">" : " ");

        display.setCursor(8, 16 + row * 12);
        display.print(name);

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

void WiFiScreen::drawEating() {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("EATING ");

    String name = eatenName;
    if (name.length() > 18) name = name.substring(0, 18);

    display.print(name);

    animator.draw();
}

void WiFiScreen::drawEatResult() {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    String name = eatenName;
    if (name.length() > 18) name = name.substring(0, 18);

    display.setCursor(0, 8);
    display.print(name);
    display.print(" EATEN!");

    display.setCursor(0, 30);
    display.print("+");
    display.print(eatResult.energyGained);
    display.print(" E");

    display.setCursor(64, 30);
    display.print("+");
    display.print(eatResult.finalXp);
    display.print(" XP");

    display.setCursor(0, 48);
    display.print("BONUS x");
    display.print(eatResult.multiplier, 2);
}

void WiFiScreen::drawLevelUp() {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(34, 0);
    display.print("LEVEL UP!");

    animator.draw();
}

void WiFiScreen::drawLevelUpResult() {
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(34, 10);
    display.print("LEVEL UP!");

    display.setTextSize(2);
    display.setCursor(34, 32);
    display.print("LV ");
    display.print(newLevel);
}

String WiFiScreen::getDisplayName(const WiFiNetwork& network) const {
    if (network.ssid.length() > 0) return network.ssid;

    return "<HIDDEN>";
}