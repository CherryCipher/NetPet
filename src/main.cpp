#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <logger/logger.h>
#include <wifi/WiFiManager.h>

#include "animation/animationmanager.h"
#include "input/inputmanager.h"
#include "pet/foodmanager.h"
#include "pet/petdata.h"
#include "pet/petmanager.h"
#include "pet/progressionmanager.h"
#include "screen/screenmanager.h"
#include "screen/wifi/wifiscreen.h"
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

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Logger logger;
WiFiManager wifi(logger);

PetData petData;
PetStorage petStorage;

AnimationManager animator(display);
InputManager input(BTN_K1, BTN_K2, BTN_K3, BTN_K4);
ProgressionManager progression(petData);
PetManager pet(animator, petData, petStorage, progression);
FoodManager food(petData, pet, progression, petStorage);

WiFiScreen wifiScreen(display, input, wifi, food);
ScreenManager screens(display, input, pet, animator, petData, progression, wifiScreen);

/**
 * @brief Initializes NetPet.
 */
void setup() {
    Serial.begin(115200);

    Wire.begin(PIN_SDA, PIN_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println("OLED initialization failed!");
        while (true) delay(100);
    }

    input.begin();

    logger.start();
    wifi.start();

    if (!petStorage.begin()) {
        Serial.println("Pet storage initialization failed!");
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

    screens.render();

    Serial.println("NetPet ready");
}

/**
 * @brief Runs the NetPet application.
 */
void loop() {
    input.update();

    animator.update();
    pet.update();
    screens.update();

    screens.render();
}