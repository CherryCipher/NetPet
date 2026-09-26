#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "../animation/animation.h"
#include "../assets/frames.h"

/**
 * @brief Controls the NetPet idle world.
 *
 * Handles fish movement, swimming animation, pauses, sleeping and bubbles.
 * Rendering is written only to the display buffer.
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
    static constexpr int16_t SCREEN_WIDTH = 128;

    static constexpr int16_t WORLD_TOP = 16;
    static constexpr int16_t WORLD_BOTTOM = 64;

    static constexpr int16_t FISH_WIDTH = 36;
    static constexpr int16_t FISH_HEIGHT = 36;

    static constexpr unsigned long MOVE_INTERVAL = 70;
    static constexpr unsigned long SWIM_FRAME_INTERVAL = 180;
    static constexpr unsigned long BUBBLE_INTERVAL = 900;
    static constexpr unsigned long BUBBLE_MOVE_INTERVAL = 120;

    static constexpr uint8_t SWIM_FRAME_COUNT = 3;
    static constexpr uint8_t MAX_BUBBLES = 4;

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

    uint8_t swimFrame = 0;

    unsigned long lastMove = 0;
    unsigned long lastSwimFrame = 0;
    unsigned long lastBubble = 0;
    unsigned long lastBubbleMove = 0;

    unsigned long nextPause = 0;
    unsigned long stateUntil = 0;

    Bubble bubbles[MAX_BUBBLES];

    void updateFish();
    void updateSwimAnimation();
    void updateBubbles();
    void spawnBubble();

    void startSwimming();
    void startPause();
    void startOffscreen();

    void resetFromOutside();
    void moveToSleepPosition();

    bool isFullyOffscreen() const;
    bool isFullyVisible() const;

    const Bitmap& getFishBitmap() const;
    const Bitmap& getSwimBitmap() const;
};