#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "../../assets/bitmap.h"
#include "../../assets/frames.h"
#include "../../input/inputmanager.h"
#include "../../pet/petdata.h"
#include "../../pet/petmanager.h"
#include "../../pet/progressionmanager.h"
#include "../../storage/petstorage.h"

/**
 * @brief Simple obstacle-dodging NetPet mini game.
 */
class MiniGameScreen {
public:
    MiniGameScreen(
        Adafruit_SSD1306& display,
        InputManager& input,
        PetManager& pet,
        PetData& petData,
        ProgressionManager& progression,
        PetStorage& storage
    );

    void begin();
    bool update();
    void draw();

private:
    static constexpr int16_t SCREEN_W = 128;
    static constexpr int16_t SCREEN_H = 64;

    static constexpr int16_t GAME_TOP = 11;
    static constexpr int16_t GAME_BOTTOM = 64;

    static constexpr int16_t FISH_X = 8;
    static constexpr int16_t FISH_STEP = 5;

    static constexpr uint8_t MAX_OBSTACLES = 3;
    static constexpr uint8_t OBSTACLE_TYPE_COUNT = 5;

    static constexpr unsigned long START_MOVE_INTERVAL = 90;
    static constexpr unsigned long MIN_MOVE_INTERVAL = 28;
    static constexpr unsigned long SPEED_STEP = 3;

    static constexpr int16_t SPAWN_MIN_DISTANCE = 35;
    static constexpr int16_t SPAWN_MAX_DISTANCE = 60;

    enum class State : uint8_t {
        PLAYING,
        GAME_OVER
    };

    struct Obstacle {
        int16_t x = 0;
        int16_t y = 0;
        uint8_t type = 0;
        bool active = false;
        bool scored = false;
    };

    Adafruit_SSD1306& display;
    InputManager& input;
    PetManager& pet;
    PetData& petData;
    ProgressionManager& progression;
    PetStorage& storage;

    State state = State::PLAYING;

    Obstacle obstacles[MAX_OBSTACLES];

    int16_t fishY = 30;

    uint32_t score = 0;
    uint32_t xpEarned = 0;

    unsigned long lastMoveAt = 0;
    unsigned long moveInterval = START_MOVE_INTERVAL;

    void resetGame();
    void updatePlaying();

    void moveFishUp();
    void moveFishDown();

    void updateObstacles();
    void spawnObstacle(uint8_t index);
    void increaseSpeed();

    bool checkCollision(const Obstacle& obstacle) const;
    void gameOver();

    void drawPlaying();
    void drawGameOver();

    void drawFish();
    void drawObstacle(const Obstacle& obstacle);

    const Bitmap& getObstacleSprite(uint8_t type) const;
};