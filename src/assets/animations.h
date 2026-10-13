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
    { &FRAME_FISH_EAT_BLE1, 100 },
    { &FRAME_FISH_EAT_BLE2, 100 },
    { &FRAME_FISH_EAT_BLE3, 100 },
    { &FRAME_FISH_EAT_BLE4, 100 },
    { &FRAME_FISH_EAT_BLE5, 100 },
    { &FRAME_FISH_EAT_BLE6, 100 },
    { &FRAME_FISH_EAT_BLE7, 100 },
    { &FRAME_FISH_EAT_BLE8, 100 },
    { &FRAME_FISH_EAT_BLE9, 100 },
    { &FRAME_FISH_EAT_BLE10, 100 },
    { &FRAME_FISH_EAT_BLE11, 100 },
    { &FRAME_FISH_EAT_BLE12, 100 },
    { &FRAME_FISH_EAT_BLE13, 100 },
    { &FRAME_FISH_EAT_BLE14, 100 },
    { &FRAME_FISH_EAT_BLE15, 100 },
    { &FRAME_FISH_EAT_BLE16, 100 },
    { &FRAME_FISH_EAT_BLE17, 500 }
};

const Animation ANIMATION_EAT_BLE = {
    EAT_BLE_FRAMES,
    sizeof(EAT_BLE_FRAMES) / sizeof(EAT_BLE_FRAMES[0]),
    false
};

/**
 * @brief BLE eating animation.
 */
const AnimationFrame EAT_ETH_FRAMES[] = {
    { &FRAME_FISH_EAT_ETH1, 100 },
    { &FRAME_FISH_EAT_ETH2, 100 },
    { &FRAME_FISH_EAT_ETH3, 100 },
    { &FRAME_FISH_EAT_ETH4, 100 },
    { &FRAME_FISH_EAT_ETH5, 100 },
    { &FRAME_FISH_EAT_ETH6, 100 },
    { &FRAME_FISH_EAT_ETH7, 100 },
    { &FRAME_FISH_EAT_ETH8, 100 },
    { &FRAME_FISH_EAT_ETH9, 100 },
    { &FRAME_FISH_EAT_ETH10, 100 },
    { &FRAME_FISH_EAT_ETH11, 100 },
    { &FRAME_FISH_EAT_ETH12, 100 },
    { &FRAME_FISH_EAT_ETH13, 100 },
    { &FRAME_FISH_EAT_ETH14, 100 },
    { &FRAME_FISH_EAT_ETH15, 100 },
    { &FRAME_FISH_EAT_ETH16, 100 },
    { &FRAME_FISH_EAT_ETH17, 500 }
};

const Animation ANIMATION_EAT_ETH = {
    EAT_ETH_FRAMES,
    sizeof(EAT_ETH_FRAMES) / sizeof(EAT_ETH_FRAMES[0]),
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

/**
 * @brief Level-up animation.
 */
const AnimationFrame CONNECT_ETH_FRAMES[] = {
    { &FRAME_ETH_PLUG_IN1, 300 },
    { &FRAME_ETH_PLUG_IN2, 100 },
    { &FRAME_ETH_PLUG_IN3, 100 },
    { &FRAME_ETH_PLUG_IN4, 300 },
    { &FRAME_ETH_PLUG_IN5, 100 },
    { &FRAME_ETH_PLUG_IN6, 100 },
    { &FRAME_ETH_PLUG_IN8, 100 },
    { &FRAME_ETH_PLUG_IN9, 100 },
    { &FRAME_ETH_PLUG_IN10, 100 },
    { &FRAME_ETH_PLUG_IN11, 800 }
};

const Animation ANIMATION_CONNECT_ETH = {
    CONNECT_ETH_FRAMES,
    sizeof(CONNECT_ETH_FRAMES) / sizeof(CONNECT_ETH_FRAMES[0]),
    false
};

/**
 * @brief DIE animation.
 */
const AnimationFrame DEATH_FRAMES[] = {
    { &FRAME_FISH_DIE1, 100 },
    { &FRAME_FISH_DIE2, 100 },
    { &FRAME_FISH_DIE3, 100 },
    { &FRAME_FISH_DIE4, 100 },
    { &FRAME_FISH_DIE5, 100 },
    { &FRAME_FISH_DIE6, 100 },
    { &FRAME_FISH_DIE7, 100 },
    { &FRAME_FISH_DIE8, 100 },
    { &FRAME_FISH_DIE9, 100 },
    { &FRAME_FISH_DIE10, 300 },
    { &FRAME_FISH_DIE11, 300 },
    { &FRAME_FISH_DIE12, 300 },
    { &FRAME_FISH_DIE13, 300 },
    { &FRAME_FISH_DIE14, 300 }
};

const Animation ANIMATION_DEATH = {
    DEATH_FRAMES,
    sizeof(DEATH_FRAMES) / sizeof(DEATH_FRAMES[0]),
    false
};