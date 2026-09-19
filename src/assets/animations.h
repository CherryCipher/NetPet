#pragma once

#include "../animation/animation.h"
#include "frames.h"


const AnimationFrame IDLE_FRAMES[] = {
    { FRAME_NORMAL_00, 5000 },
    { FRAME_NORMAL_01_BLINK, 200 }
};

const Animation ANIMATION_IDLE = {
    IDLE_FRAMES,
    sizeof(IDLE_FRAMES) / sizeof(IDLE_FRAMES[0]),
    true
};


const AnimationFrame SLEEP_FRAMES[] = {
    { FRAME_SLEEP_00, 1000 },
    { FRAME_SLEEP_01, 500 }
};

const Animation ANIMATION_SLEEP = {
    SLEEP_FRAMES,
    sizeof(SLEEP_FRAMES) / sizeof(SLEEP_FRAMES[0]),
    true
};


