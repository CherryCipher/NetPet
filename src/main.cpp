#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "animation/animationmanager.h"
#include "input/inputmanager.h"
#include "pet/petmanager.h"
#include "screen/screenmanager.h"

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

AnimationManager animator(display);
InputManager input(BTN_K1, BTN_K2, BTN_K3, BTN_K4);
PetManager pet(animator);
ScreenManager screens(display, input, pet, animator);

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