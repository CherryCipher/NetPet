#pragma once

#include <Arduino.h>

/**
 * @brief Describes a monochrome bitmap stored in flash.
 */
struct Bitmap {
    const uint8_t* data;
    uint16_t width;
    uint16_t height;
};