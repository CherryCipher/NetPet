#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "animation.h"

/**
 * @brief Controls non-blocking bitmap animation playback.
 *
 * AnimationManager manages animation timing and draws the current frame into
 * the display buffer. It never sends the buffer to the physical display.
 */
class AnimationManager {
public:
    /**
     * @brief Creates a new AnimationManager.
     *
     * @param display Reference to the OLED display buffer used for rendering.
     */
    explicit AnimationManager(Adafruit_SSD1306& display);

    /**
     * @brief Starts an animation.
     *
     * @param animation Animation to play.
     * @param restart Restart the animation when it is already active.
     */
    void play(const Animation& animation, bool restart = false);

    /**
     * @brief Stops the current animation.
     */
    void stop();

    /**
     * @brief Updates animation timing.
     */
    void update();

    /**
     * @brief Draws the current animation frame at the given position.
     *
     * @param x Horizontal position.
     * @param y Vertical position.
     */
    void draw(int16_t x = 0, int16_t y = 0) const;

    /**
     * @brief Returns whether an animation is currently playing.
     */
    bool isPlaying() const;

    /**
     * @brief Returns whether a non-looping animation has finished.
     */
    bool isFinished() const;

private:
    Adafruit_SSD1306& display;

    const Animation* currentAnimation = nullptr;

    uint8_t currentFrame = 0;
    unsigned long frameStartedAt = 0;

    bool playing = false;
    bool finished = false;
};