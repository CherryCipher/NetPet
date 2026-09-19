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
    false
};

const AnimationFrame DEATH_FRAMES[] = {
    { FRAME_DEATH_00, 100 },
    { FRAME_DEATH_01, 1000 },
    { FRAME_DEATH_02, 4000 }
};

const Animation ANIMATION_DEATH = {
    DEATH_FRAMES,
    sizeof(DEATH_FRAMES) / sizeof(DEATH_FRAMES[0]),
    false
};

const AnimationFrame EAT_FRAMES[] = {
    { FRAME_EAT_00, 200 },
    { FRAME_EAT_01, 200 },
    { FRAME_EAT_02, 200 },
    { FRAME_EAT_03, 500 },
    { FRAME_EAT_04, 500 },
    { FRAME_EAT_03, 500 },
    { FRAME_EAT_04, 500 },
    { FRAME_EAT_03, 500 },
    { FRAME_EAT_04, 500 },
    { FRAME_EAT_03, 500 },
    { FRAME_EAT_04, 500 }
};

const Animation ANIMATION_EAT = {
    EAT_FRAMES,
    sizeof(EAT_FRAMES) / sizeof(EAT_FRAMES[0]),
    false
};

const AnimationFrame LEVEL_UP_FRAMES[] = {
    { FRAME_LEVELUP_00, 200 },
    { FRAME_LEVELUP_01, 200 }
};

const Animation ANIMATION_LEVEL_UP = {
    LEVEL_UP_FRAMES,
    sizeof(LEVEL_UP_FRAMES) / sizeof(LEVEL_UP_FRAMES[0]),
    false
};


