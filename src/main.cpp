#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "animation/animationmanager.h"
#include "input/inputmanager.h"
#include "pet/petmanager.h"

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
InputManager buttons(BTN_K1, BTN_K2, BTN_K3, BTN_K4);
PetManager pet(animator);

/**
 * @brief Draws the NetPet status area.
 */
void drawStatus() {
    display.fillRect(0, 0, SCREEN_WIDTH, 16, SSD1306_BLACK);

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("ENERGY: 100%");

    display.setCursor(0, 8);
    display.print("NO CONNECTIONS...");
}

/**
 * @brief Initializes NetPet hardware and managers.
 */
void setup() {
    Serial.begin(115200);

    Wire.begin(PIN_SDA, PIN_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println("OLED initialization failed!");
        while (true) delay(100);
    }

    buttons.begin();

    display.clearDisplay();
    drawStatus();
    display.display();

    pet.begin();

    Serial.println("NetPet ready");
}

/**
 * @brief Runs the NetPet application loop.
 */
void loop() {
    buttons.update();

    if (buttons.anyPressed()) pet.activity();

    pet.update();
    animator.update();
}