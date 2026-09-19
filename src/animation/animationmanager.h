#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "animation.h"

/**
 * @brief Controls non-blocking bitmap animations on the OLED display.
 *
 * The AnimationManager uses millis() for frame timing and therefore does not
 * block the main application loop. Animations are rendered only inside the
 * dedicated face area below the status bar.
 */
class AnimationManager {
public:
    /**
     * @brief Creates a new AnimationManager.
     *
     * @param display Reference to the OLED display used for rendering.
     */
    explicit AnimationManager(Adafruit_SSD1306& display);

    /**
     * @brief Starts playing an animation.
     *
     * @param animation Animation to play.
     * @param restart Restart the animation if it is already playing.
     */
    void play(const Animation& animation, bool restart = false);

    /**
     * @brief Stops the currently playing animation.
     */
    void stop();

    /**
     * @brief Updates the animation state and advances frames when required.
     *
     * This method should be called as frequently as possible from loop().
     */
    void update();

    /**
     * @brief Draws the current animation frame to the display.
     *
     * This can be used to redraw the current frame after another part of the
     * application has modified the display buffer.
     */
    void draw();

    /**
     * @brief Checks whether an animation is currently playing.
     *
     * @return true if an animation is playing, otherwise false.
     */
    bool isPlaying() const;

private:
    static constexpr int FACE_X = 0;
    static constexpr int FACE_Y = 16;
    static constexpr int FACE_WIDTH = 128;
    static constexpr int FACE_HEIGHT = 48;

    Adafruit_SSD1306& display;

    const Animation* currentAnimation = nullptr;

    uint8_t currentFrame = 0;
    unsigned long frameStartedAt = 0;
    bool playing = false;

    /**
     * @brief Advances the animation to the next frame.
     */
    void nextFrame();
};