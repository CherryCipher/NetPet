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

const AnimationFrame CONNECT_FRAMES[] = {
    { FRAME_ETHERNET_00, 500 },
    { FRAME_ETHERNET_01, 100 },
    { FRAME_ETHERNET_02, 200 },
    { FRAME_ETHERNET_03, 500 },
    { FRAME_ETHERNET_02, 100 },
    { FRAME_ETHERNET_04, 100 },
    { FRAME_ETHERNET_05, 100 },
    { FRAME_ETHERNET_06, 200 },
    { FRAME_ETHERNET_07, 200 },
    { FRAME_ETHERNET_08, 1000 }
};

const Animation ANIMATION_CONNECT = {
    CONNECT_FRAMES,
    sizeof(CONNECT_FRAMES) / sizeof(CONNECT_FRAMES[0]),
    true
};


