#include "blescreen.h"

BLEScreen::BLEScreen(
    Adafruit_SSD1306& display,
    InputManager& input,
    BLEManager& ble,
    FoodManager& food,
    PetManager& pet,
    ProgressionManager& progression,
    AnimationManager& animator
)
    : display(display),
      input(input),
      ble(ble),
      food(food),
      pet(pet),
      progression(progression),
      animator(animator) {}

void BLEScreen::begin() {
    selectedIndex = 0;
    scrollOffset = 0;
    startScan();
}

bool BLEScreen::update() {
    switch (state) {
        case State::SCANNING:
            if (input.wasPressed(Button::K4)) {
                ble.stopScan();
                return true;
            }

            if (millis() - stateStartedAt >= SCAN_DURATION) finishScan();
            return false;

        case State::EATING:
            if (animator.isFinished()) finishEating();
            return false;

        case State::EAT_RESULT:
            if (millis() - stateStartedAt >= EAT_RESULT_DURATION) {
                if (levelUpPending) startLevelUp();
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
        if (input.wasPressed(Button::K3)) startScan();
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

void BLEScreen::draw() {
    switch (state) {
        case State::SCANNING: drawScanning(); break;
        case State::LIST: drawList(); break;
        case State::EMPTY: drawEmpty(); break;
        case State::ERROR: drawError(); break;
        case State::EATING: drawEating(); break;
        case State::EAT_RESULT: drawEatResult(); break;
        case State::LEVEL_UP: drawLevelUp(); break;
        case State::LEVEL_UP_RESULT: drawLevelUpResult(); break;
    }
}

void BLEScreen::startScan() {
    deviceCount = 0;
    selectedIndex = 0;
    scrollOffset = 0;

    if (!ble.startScan()) {
        state = State::ERROR;
        return;
    }

    stateStartedAt = millis();
    state = State::SCANNING;
}

void BLEScreen::finishScan() {
    ble.stopScan();
    rebuildDeviceList();
}

void BLEScreen::rebuildDeviceList() {
    deviceCount = 0;

    const uint8_t scanCount = ble.getDeviceCount();

    for (uint8_t i = 0; i < scanCount && deviceCount < MAX_DEVICES; i++) {
        const BLEDeviceInfo& device = ble.getDevice(i);

        if (food.isBLEEaten(device.address)) continue;

        deviceIndices[deviceCount] = i;
        deviceCount++;
    }

    selectedIndex = 0;
    scrollOffset = 0;

    state = deviceCount > 0 ? State::LIST : State::EMPTY;
}

void BLEScreen::moveUp() {
    if (deviceCount == 0) return;

    if (selectedIndex == 0) selectedIndex = deviceCount - 1;
    else selectedIndex--;

    if (selectedIndex < scrollOffset) scrollOffset = selectedIndex;

    if (selectedIndex >= scrollOffset + MAX_VISIBLE_ITEMS) {
        scrollOffset = selectedIndex - MAX_VISIBLE_ITEMS + 1;
    }
}

void BLEScreen::moveDown() {
    if (deviceCount == 0) return;

    selectedIndex = (selectedIndex + 1) % deviceCount;

    if (selectedIndex < scrollOffset) scrollOffset = selectedIndex;

    if (selectedIndex >= scrollOffset + MAX_VISIBLE_ITEMS) {
        scrollOffset = selectedIndex - MAX_VISIBLE_ITEMS + 1;
    }
}

void BLEScreen::startEating() {
    if (deviceCount == 0 || selectedIndex >= deviceCount) return;

    const BLEDeviceInfo& device = ble.getDevice(deviceIndices[selectedIndex]);

    eatenName = getDisplayName(device);
    previousLevel = progression.getLevel();

    pet.pauseEnergyDecay();

    eatResult = food.eatBLE(device.address, device.rssi);

    newLevel = progression.getLevel();
    levelUpPending = newLevel > previousLevel;

    Serial.print("BLE eaten: ");
    Serial.print(eatenName);
    Serial.print(" | ");
    Serial.print(device.address);
    Serial.print(" | XP +");
    Serial.print(eatResult.finalXp);
    Serial.print(" | Level ");
    Serial.print(previousLevel);
    Serial.print(" -> ");
    Serial.print(newLevel);
    Serial.print(" | Level up: ");
    Serial.println(levelUpPending ? "YES" : "NO");

    animator.play(ANIMATION_EAT_BLE, true);
    state = State::EATING;
}

void BLEScreen::finishEating() {
    stateStartedAt = millis();
    state = State::EAT_RESULT;
}

void BLEScreen::startLevelUp() {
    Serial.print("Starting level-up sequence: ");
    Serial.print(previousLevel);
    Serial.print(" -> ");
    Serial.println(newLevel);

    animator.play(ANIMATION_LEVEL_UP, true);

    stateStartedAt = millis();
    state = State::LEVEL_UP;
}

void BLEScreen::finishFlow() {
    pet.resumeEnergyDecay();

    levelUpPending = false;

    rebuildDeviceList();
}

void BLEScreen::drawScanning() {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(4, 14);
    display.print("SEARCHING FOR FOOD");

    display.setCursor(28, 30);
    display.print("BLE SCANNING");

    display.setCursor(42, 46);

    const uint8_t dots = ((millis() - stateStartedAt) / 300) % 4;
    for (uint8_t i = 0; i < dots; i++) display.print(".");
}

void BLEScreen::drawList() {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("BLE FOOD ");
    display.print(selectedIndex + 1);
    display.print("/");
    display.print(deviceCount);

    for (uint8_t row = 0; row < MAX_VISIBLE_ITEMS; row++) {
        const uint8_t listIndex = scrollOffset + row;
        if (listIndex >= deviceCount) break;

        const BLEDeviceInfo& device = ble.getDevice(deviceIndices[listIndex]);

        String name = getDisplayName(device);
        if (name.length() > 12) name = name.substring(0, 12);

        display.setCursor(0, 16 + row * 12);
        display.print(listIndex == selectedIndex ? ">" : " ");

        display.setCursor(8, 16 + row * 12);
        display.print(name);

        display.setCursor(92, 16 + row * 12);
        display.print(device.rssi);
    }
}

void BLEScreen::drawEmpty() {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("BLE FOOD");

    display.setCursor(0, 22);
    display.print("NO FOOD FOUND");

    display.setCursor(0, 40);
    display.print("K3 RESCAN");

    display.setCursor(0, 54);
    display.print("K4 BACK");
}

void BLEScreen::drawError() {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("BLE FOOD");

    display.setCursor(0, 22);
    display.print("SCAN FAILED");

    display.setCursor(0, 40);
    display.print("K3 RETRY");

    display.setCursor(0, 54);
    display.print("K4 BACK");
}

void BLEScreen::drawEating() {
    animator.draw();
}

void BLEScreen::drawEatResult() {
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

void BLEScreen::drawLevelUp() {
    animator.draw();
}

void BLEScreen::drawLevelUpResult() {
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(34, 10);
    display.print("LEVEL UP!");

    display.setTextSize(2);
    display.setCursor(34, 32);
    display.print("LV ");
    display.print(newLevel);
}

String BLEScreen::getDisplayName(const BLEDeviceInfo& device) const {
    if (!device.name.isEmpty()) return device.name;

    String address = device.address;
    address.toUpperCase();

    return address;
}