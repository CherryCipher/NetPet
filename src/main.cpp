#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "animation/animationmanager.h"
#include "assets/animations.h"

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

/**
 * @brief Draws the two-line NetPet status area.
 *
 * The status area occupies the top 16 pixels of the display. The remaining
 * 48 pixels are reserved for the animated face.
 */
void drawStatus() {
    display.fillRect(0, 0, SCREEN_WIDTH, 16, SSD1306_BLACK);

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("FOOD: 100%");

    display.setCursor(0, 8);
    display.print("NETPET READY");
}

/**
 * @brief Initializes the NetPet hardware and display.
 */
void setup() {
    Serial.begin(115200);

    pinMode(BTN_K1, INPUT_PULLUP);
    pinMode(BTN_K2, INPUT_PULLUP);
    pinMode(BTN_K3, INPUT_PULLUP);
    pinMode(BTN_K4, INPUT_PULLUP);

    Wire.begin(PIN_SDA, PIN_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println("OLED initialization failed!");
        while (true) delay(100);
    }

    display.clearDisplay();

    drawStatus();
    display.display();

    animator.play(ANIMATION_TEST);

    Serial.println("NetPet ready");
}

/**
 * @brief Runs the NetPet application loop.
 */
void loop() {
    animator.update();
}