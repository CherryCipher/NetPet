#include "screenmanager.h"

ScreenManager::ScreenManager(
    Adafruit_SSD1306& display,
    InputManager& input,
    PetManager& pet,
    PetIdle& petIdle,
    AnimationManager& animator,
    PetData& petData,
    ProgressionManager& progression,
    WiFiScreen& wifiScreen,
    BLEScreen& bleScreen
)
    : display(display),
      input(input),
      pet(pet),
      petIdle(petIdle),
      animator(animator),
      petData(petData),
      progression(progression),
      wifiScreen(wifiScreen),
      bleScreen(bleScreen) {}

void ScreenManager::begin() {
    currentScreen = ScreenId::PET;
    pet.enterPetScreen();
    petIdle.begin();
}

void ScreenManager::update() {
    if (pet.getState() == PetState::DEAD && currentScreen != ScreenId::PET) {
        currentScreen = ScreenId::PET;
        return;
    }

    switch (currentScreen) {
        case ScreenId::PET: updatePet(); break;
        case ScreenId::MENU: updateMenu(); break;
        case ScreenId::STATS: updateStats(); break;
        case ScreenId::WIFI: updateWiFi(); break;
        case ScreenId::BLE: updateBLE(); break;
    }
}

void ScreenManager::render() {
    display.clearDisplay();

    switch (currentScreen) {
        case ScreenId::PET: drawPet(); break;
        case ScreenId::MENU: drawMenu(); break;
        case ScreenId::STATS: drawStats(); break;
        case ScreenId::WIFI: wifiScreen.draw(); break;
        case ScreenId::BLE: bleScreen.draw(); break;
    }

    display.display();
}

void ScreenManager::show(ScreenId screen) {
    if (currentScreen == screen) return;

    currentScreen = screen;

    if (currentScreen == ScreenId::PET) {
        pet.enterPetScreen();
        petIdle.begin();
        return;
    }

    pet.setSleepEnabled(false);

    if (currentScreen == ScreenId::WIFI) wifiScreen.begin();
    if (currentScreen == ScreenId::BLE) bleScreen.begin();
}

ScreenId ScreenManager::getCurrentScreen() const {
    return currentScreen;
}

void ScreenManager::updatePet() {
    if (input.wasPressed(Button::K4)) {
        show(ScreenId::MENU);
        return;
    }

    if (input.anyPressed()) pet.activity();

    petIdle.setSick(pet.getEnergy() <= 25);

    if (pet.getState() == PetState::SLEEP) petIdle.sleep();
    else petIdle.wake();

    if (pet.getState() == PetState::IDLE || pet.getState() == PetState::SLEEP) petIdle.update();
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

void ScreenManager::updateStats() {
    if (input.wasPressed(Button::K4)) show(ScreenId::MENU);
}

void ScreenManager::updateWiFi() {
    if (wifiScreen.update()) show(ScreenId::MENU);
}

void ScreenManager::updateBLE() {
    if (bleScreen.update()) show(ScreenId::MENU);
}

void ScreenManager::selectMenuItem() {
    switch (selectedMenuItem) {
        case 0: show(ScreenId::STATS); break;
        case 1: show(ScreenId::WIFI); break;
        case 2: show(ScreenId::BLE); break;
    }
}

void ScreenManager::drawPet() {
    if (pet.getState() == PetState::CONNECTING || pet.getState() == PetState::DEAD) {
        animator.draw();
        return;
    }

    drawStatus();
    petIdle.draw();
}

void ScreenManager::drawStatus() {
    const unsigned long uptimeSeconds = progression.getUptime() / 1000;

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("E:");
    display.print(petData.energy);
    display.print("%");

    display.setCursor(58, 0);
    display.print("LV:");
    display.print(progression.getLevel());

    display.setCursor(0, 8);
    display.print("UP:");
    display.print(uptimeSeconds);
    display.print("s");

    display.setCursor(58, 8);
    display.print("x");
    display.print(progression.getUptimeMultiplier(), 2);
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

void ScreenManager::drawStats() {
    const unsigned long uptimeSeconds = progression.getUptime() / 1000;
    const unsigned long hours = uptimeSeconds / 3600;
    const unsigned long minutes = (uptimeSeconds % 3600) / 60;

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("NETPET STATS");

    display.setCursor(0, 12);
    display.print("LV: ");
    display.print(progression.getLevel());
    display.print("  XP: ");
    display.print(progression.getXp());

    display.setCursor(0, 22);
    display.print("ENERGY: ");
    display.print(petData.energy);
    display.print("%");

    display.setCursor(0, 32);
    display.print("UPTIME: ");

    if (hours < 10) display.print("0");
    display.print(hours);
    display.print(":");
    if (minutes < 10) display.print("0");
    display.print(minutes);

    display.setCursor(0, 42);
    display.print("BONUS: x");
    display.print(progression.getUptimeMultiplier(), 2);

    display.setCursor(0, 52);
    display.print("WIFI:");
    display.print(petData.wifiEaten);
    display.print(" BLE:");
    display.print(petData.bleEaten);
}