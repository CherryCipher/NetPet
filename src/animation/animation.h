#pragma once

#include <Arduino.h>
#include "../assets/bitmap.h"

/**
 * @brief Represents a single bitmap frame in an animation.
 */
struct AnimationFrame {
    const Bitmap* bitmap;      ///< Bitmap displayed for this frame.
    uint16_t duration;         ///< Duration of the frame in milliseconds.
};

/**
 * @brief Describes a complete bitmap animation.
 */
struct Animation {
    const AnimationFrame* frames; ///< Pointer to the animation frame array.
    uint8_t frameCount;           ///< Number of frames in the animation.
    bool loop;                    ///< Whether the animation should loop.
};