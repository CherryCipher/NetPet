#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../assets/frames.h"

/**
 * @brief Controls the visual behavior of the pet on the idle screen.
 *
 * PetIdle only handles movement and rendering. It does not manage
 * gameplay state such as energy, XP or progression.
 */
class PetIdle {
public:
    explicit PetIdle(Adafruit_SSD1306& display);

    void begin();
    void update();
    void draw() const;

    void sleep();
    void wake();
    void setSick(bool sick);

private:
    static constexpr int SCREEN_WIDTH = 128;

    static constexpr int WORLD_TOP = 16;
    static constexpr int WORLD_BOTTOM = 64;

    static constexpr int FISH_WIDTH = 36;
    static constexpr int FISH_HEIGHT = 36;

    static constexpr unsigned long MOVE_INTERVAL = 35;
    static constexpr unsigned long BUBBLE_INTERVAL = 900;
    static constexpr unsigned long BUBBLE_MOVE_INTERVAL = 100;

    static constexpr uint8_t BUBBLE_COUNT = 4;

    enum class IdleState : uint8_t {
        SWIMMING,
        PAUSED,
        OFFSCREEN
    };

    struct Bubble {
        int16_t x = 0;
        int16_t y = 0;
        bool active = false;
    };

    Adafruit_SSD1306& display;

    int16_t x = 0;
    int16_t y = 0;
    int8_t direction = 1;

    bool sleeping = false;
    bool sick = false;

    IdleState idleState = IdleState::SWIMMING;

    unsigned long lastMove = 0;
    unsigned long lastBubble = 0;
    unsigned long lastBubbleMove = 0;
    unsigned long stateUntil = 0;
    unsigned long nextPause = 0;

    Bubble bubbles[BUBBLE_COUNT];

    void updateFish();
    void updateBubbles();
    void spawnBubble();

    void startSwimming();
    void startPause();
    void startOffscreen();
    void resetFromOutside();

    bool isFullyOffscreen() const;
    bool isFullyVisible() const;

    const Bitmap& getFishBitmap() const;
};