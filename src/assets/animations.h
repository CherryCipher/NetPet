#pragma once

#include "../animation/animation.h"
#include "frames.h"

/**
 * @brief WiFi eating animation.
 */
const AnimationFrame EAT_WIFI_FRAMES[] = {
    { &FRAME_FISH_EAT_WIFI1, 100 },
    { &FRAME_FISH_EAT_WIFI2, 100 },
    { &FRAME_FISH_EAT_WIFI3, 100 },
    { &FRAME_FISH_EAT_WIFI4, 100 },
    { &FRAME_FISH_EAT_WIFI5, 100 },
    { &FRAME_FISH_EAT_WIFI6, 100 },
    { &FRAME_FISH_EAT_WIFI7, 100 },
    { &FRAME_FISH_EAT_WIFI8, 100 },
    { &FRAME_FISH_EAT_WIFI9, 100 },
    { &FRAME_FISH_EAT_WIFI10, 100 },
    { &FRAME_FISH_EAT_WIFI11, 100 },
    { &FRAME_FISH_EAT_WIFI12, 100 },
    { &FRAME_FISH_EAT_WIFI13, 100 },
    { &FRAME_FISH_EAT_WIFI14, 100 },
    { &FRAME_FISH_EAT_WIFI15, 100 },
    { &FRAME_FISH_EAT_WIFI16, 100 },
    { &FRAME_FISH_EAT_WIFI17, 500 }
};

const Animation ANIMATION_EAT_WIFI = {
    EAT_WIFI_FRAMES,
    sizeof(EAT_WIFI_FRAMES) / sizeof(EAT_WIFI_FRAMES[0]),
    false
};

/**
 * @brief BLE eating animation.
 */
const AnimationFrame EAT_BLE_FRAMES[] = {
    { &FRAME_FISH_EAT_WIFI1, 100 },
    { &FRAME_FISH_EAT_WIFI2, 100 },
    { &FRAME_FISH_EAT_WIFI3, 100 },
    { &FRAME_FISH_EAT_WIFI4, 100 },
    { &FRAME_FISH_EAT_WIFI5, 100 },
    { &FRAME_FISH_EAT_WIFI6, 100 },
    { &FRAME_FISH_EAT_WIFI7, 100 },
    { &FRAME_FISH_EAT_WIFI8, 100 },
    { &FRAME_FISH_EAT_WIFI9, 100 },
    { &FRAME_FISH_EAT_WIFI10, 100 },
    { &FRAME_FISH_EAT_WIFI11, 100 },
    { &FRAME_FISH_EAT_WIFI12, 100 },
    { &FRAME_FISH_EAT_WIFI13, 100 },
    { &FRAME_FISH_EAT_WIFI14, 100 },
    { &FRAME_FISH_EAT_WIFI15, 100 },
    { &FRAME_FISH_EAT_WIFI16, 100 },
    { &FRAME_FISH_EAT_WIFI17, 500 }
};

const Animation ANIMATION_EAT_BLE = {
    EAT_BLE_FRAMES,
    sizeof(EAT_BLE_FRAMES) / sizeof(EAT_BLE_FRAMES[0]),
    false
};

/**
 * @brief Level-up animation.
 */
const AnimationFrame LEVEL_UP_FRAMES[] = {
    { &FRAME_FISH_LEVELUP1, 100 },
    { &FRAME_FISH_LEVELUP2, 100 },
    { &FRAME_FISH_LEVELUP3, 100 },
    { &FRAME_FISH_LEVELUP4, 100 },
    { &FRAME_FISH_LEVELUP5, 100 },
    { &FRAME_FISH_LEVELUP6, 100 },
    { &FRAME_FISH_LEVELUP7, 100 },
    { &FRAME_FISH_LEVELUP8, 100 },
    { &FRAME_FISH_LEVELUP9, 100 },
    { &FRAME_FISH_LEVELUP10, 100 },
    { &FRAME_FISH_LEVELUP11, 100 },
    { &FRAME_FISH_LEVELUP12, 100 },
    { &FRAME_FISH_LEVELUP13, 700 }
};

const Animation ANIMATION_LEVEL_UP = {
    LEVEL_UP_FRAMES,
    sizeof(LEVEL_UP_FRAMES) / sizeof(LEVEL_UP_FRAMES[0]),
    false
};