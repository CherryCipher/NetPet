#include "screenmanager.h"

ScreenManager::ScreenManager(Adafruit_SSD1306& display, InputManager& input, PetManager& pet, AnimationManager& animator)
    : display(display), input(input), pet(pet), animator(animator) {}

void ScreenManager::begin() {
    currentScreen = ScreenId::PET;
}

void ScreenManager::update() {
    switch (currentScreen) {
        case ScreenId::PET:
            updatePet();
            break;

        case ScreenId::MENU:
            updateMenu();
            break;

        case ScreenId::STATS:
        case ScreenId::WIFI:
        case ScreenId::BLE:
            updatePlaceholder();
            break;
    }
}

void ScreenManager::render() {
    display.clearDisplay();

    switch (currentScreen) {
        case ScreenId::PET:
            drawPet();
            break;

        case ScreenId::MENU:
            drawMenu();
            break;

        case ScreenId::STATS:
        case ScreenId::WIFI:
        case ScreenId::BLE:
            drawPlaceholder();
            break;
    }

    display.display();
}

void ScreenManager::show(ScreenId screen) {
    if (currentScreen == screen) return;

    currentScreen = screen;

    if (currentScreen == ScreenId::PET) pet.activity();
}

ScreenId ScreenManager::getCurrentScreen() const {
    return currentScreen;
}

void ScreenManager::updatePet() {
    if (input.wasPressed(Button::K4)) {
        pet.activity();
        show(ScreenId::MENU);
        return;
    }

/*     if (input.wasPressed(Button::K3)) {
        pet.connect();
        return;
    } */

    if (input.anyPressed()) pet.activity();
}

void ScreenManager::updateMenu() {
    if (input.wasPressed(Button::K1)) {
        selectedMenuItem = selectedMenuItem == 0 ? MENU_ITEM_COUNT - 1 : selectedMenuItem - 1;
        return;
    }

    if (input.wasPressed(Button::K2)) {
        selectedMenuItem = (selectedMenuItem + 1) % MENU_ITEM_COUNT;
        return;
    }

    if (input.wasPressed(Button::K3)) {
        selectMenuItem();
        return;
    }

    if (input.wasPressed(Button::K4)) show(ScreenId::PET);
}

void ScreenManager::updatePlaceholder() {
    if (input.wasPressed(Button::K4)) show(ScreenId::MENU);
}

void ScreenManager::selectMenuItem() {
    switch (selectedMenuItem) {
        case 0:
            show(ScreenId::STATS);
            break;

        case 1:
            show(ScreenId::WIFI);
            break;

        case 2:
            show(ScreenId::BLE);
            break;
    }
}

void ScreenManager::drawPet() {
    drawStatus();
    animator.draw();
}

void ScreenManager::drawStatus() {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("ENERGY: 100%");

    display.setCursor(0, 8);
    display.print("LV: 1  UP: 00:00");
}

void ScreenManager::drawMenu() {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("NETPET MENU");

    const char* items[MENU_ITEM_COUNT] = {
        "STATS",
        "SCAN WIFI",
        "SCAN BLE"
    };

    for (uint8_t i = 0; i < MENU_ITEM_COUNT; i++) {
        display.setCursor(0, 24 + i * 12);
        display.print(i == selectedMenuItem ? "> " : "  ");
        display.print(items[i]);
    }
}

void ScreenManager::drawPlaceholder() {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);

    switch (currentScreen) {
        case ScreenId::STATS:
            display.print("STATS");
            break;

        case ScreenId::WIFI:
            display.print("SCAN WIFI");
            break;

        case ScreenId::BLE:
            display.print("SCAN BLE");
            break;

        default:
            return;
    }

    display.setCursor(0, 24);
    display.print("COMING SOON");

    display.setCursor(0, 56);
    display.print("K4 BACK");
}