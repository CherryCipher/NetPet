#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <logger/logger.h>
#include <wifi/WiFiManager.h>
#include <ble/BLEManager.h>

#include "animation/animationmanager.h"
#include "ethernet/ethernetmanager.h"
#include "input/inputmanager.h"
#include "pet/foodmanager.h"
#include "pet/petdata.h"
#include "pet/petidle.h"
#include "pet/petmanager.h"
#include "pet/progressionmanager.h"
#include "screen/screenmanager.h"
#include "screen/wifi/wifiscreen.h"
#include "screen/ble/blescreen.h"
#include "screen/ethernet/ethernetscreen.h"
#include "storage/petstorage.h"

constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;
constexpr uint8_t OLED_ADDRESS = 0x3C;

constexpr uint8_t PIN_SCL = 9;
constexpr uint8_t PIN_SDA = 10;

constexpr uint8_t BTN_K1 = 7;
constexpr uint8_t BTN_K2 = 6;
constexpr uint8_t BTN_K3 = 5;
constexpr uint8_t BTN_K4 = 4;

constexpr uint8_t BTN_FACTORY_RESET = 8;
constexpr unsigned long FACTORY_RESET_DURATION = 3000;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Logger logger;
WiFiManager wifi(logger);
BLEManager ble(logger);

EthernetManager ethernet;

PetData petData;
PetStorage petStorage;

AnimationManager animator(display);
PetIdle petIdle(display);
InputManager input(BTN_K1, BTN_K2, BTN_K3, BTN_K4);

ProgressionManager progression(petData);
PetManager pet(animator, petData, petStorage, progression);
FoodManager food(petData, pet, progression, petStorage);

WiFiScreen wifiScreen(display, input, wifi, food, pet, progression, animator);
BLEScreen bleScreen(display, input, ble, food, pet, progression, animator);
EthernetScreen ethernetScreen(display, ethernet);

ScreenManager screens(
    display,
    input,
    pet,
    petIdle,
    animator,
    petData,
    progression,
    wifiScreen,
    bleScreen,
    ethernetScreen
);

/**
 * @brief Performs a complete factory reset when GPIO8 is held during boot.
 */
void checkFactoryReset() {
    pinMode(BTN_FACTORY_RESET, INPUT_PULLUP);

    if (digitalRead(BTN_FACTORY_RESET) != LOW) return;

    Serial.println("[RESET] Factory reset button detected.");

    const unsigned long startedAt = millis();

    while (millis() - startedAt < FACTORY_RESET_DURATION) {
        if (digitalRead(BTN_FACTORY_RESET) != LOW) {
            Serial.println("[RESET] Cancelled.");
            return;
        }

        display.clearDisplay();
        display.setTextColor(SSD1306_WHITE);
        display.setTextSize(1);

        display.setCursor(12, 12);
        display.print("FACTORY RESET");

        display.setCursor(12, 30);
        display.print("HOLD BUTTON...");

        const uint8_t progress = ((millis() - startedAt) * 100) / FACTORY_RESET_DURATION;

        display.drawRect(12, 48, 104, 8, SSD1306_WHITE);
        display.fillRect(14, 50, (100 * progress) / 100, 4, SSD1306_WHITE);

        display.display();

        delay(25);
    }

    Serial.println("[RESET] Clearing NetPet save.");

    petStorage.clear();
    petData = PetData();
    petStorage.save(petData);

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(20, 24);
    display.print("RESET COMPLETE");

    display.display();

    delay(1500);

    ESP.restart();
}

/**
 * @brief Handles newly discovered Ethernet gateways.
 */
void handleEthernetFood() {
    if (!ethernet.hasNewGateway()) return;

    if (pet.getState() == PetState::DEAD) return;

    const String gatewayMAC = ethernet.getGatewayMAC();

    if (gatewayMAC.isEmpty()) return;

    Serial.print("[NETPET] Gateway discovered: ");
    Serial.println(gatewayMAC);

    if (food.isEthernetEaten(gatewayMAC)) {
        Serial.println("[NETPET] Gateway already eaten.");
        return;
    }

    const uint16_t previousLevel = progression.getLevel();

    const EatResult result = food.eatEthernet(gatewayMAC);

    if (result.finalXp == 0) return;

    const uint16_t newLevel = progression.getLevel();
    const bool levelUp = newLevel > previousLevel;

    pet.activity();

    ethernetScreen.showFood(result, gatewayMAC, levelUp, newLevel);

    Serial.print("[NETPET] Ethernet food: +");
    Serial.print(result.finalXp);
    Serial.print(" XP, +");
    Serial.print(result.energyGained);
    Serial.println(" energy.");
}

void setup() {
    Serial.begin(115200);

    Wire.begin(PIN_SDA, PIN_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println("OLED initialization failed!");
        while (true) delay(100);
    }

    input.begin();

    if (!petStorage.begin()) {
        Serial.println("Pet storage initialization failed!");
        while (true) delay(100);
    }

    checkFactoryReset();

    logger.start();
    wifi.start();

    if (!ble.start()) {
        Serial.println("BLE initialization failed!");
        while (true) delay(100);
    }

    if (!food.begin()) {
        Serial.println("Food storage initialization failed!");
        while (true) delay(100);
    }

    if (!petStorage.load(petData)) {
        Serial.println("Creating new NetPet save.");
        petStorage.save(petData);
    } else {
        Serial.println("NetPet save loaded.");
    }

    progression.begin();
    pet.begin();

    screens.begin();

    if (!ethernet.begin()) {
        Serial.println("[NETPET] Ethernet unavailable.");
    }

    screens.render();

    Serial.println("NetPet ready.");
}

void loop() {
    input.update();

    ethernet.update();

    animator.update();
    pet.update();

    handleEthernetFood();

    screens.update();
    screens.render();
}