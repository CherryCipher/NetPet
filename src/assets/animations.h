#pragma once

#include "../animation/animation.h"
#include "frames.h"

/**
 * @brief Frames used by the test animation.
 */
const AnimationFrame TEST_FRAMES[] = {
    { FRAME_TESTFRAME, 1000 }
};

/**
 * @brief Test animation used to verify bitmap rendering.
 */
const Animation ANIMATION_TEST = {
    TEST_FRAMES,
    sizeof(TEST_FRAMES) / sizeof(TEST_FRAMES[0]),
    true
};