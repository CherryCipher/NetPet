#include "minigamescreen.h"

MiniGameScreen::MiniGameScreen(
    Adafruit_SSD1306& display,
    InputManager& input,
    PetManager& pet,
    PetData& petData,
    ProgressionManager& progression,
    PetStorage& storage
)
    : display(display),
      input(input),
      pet(pet),
      petData(petData),
      progression(progression),
      storage(storage) {}

void MiniGameScreen::begin() {
    resetGame();

    pet.setSleepEnabled(false);
    pet.activity();
}

bool MiniGameScreen::update() {
    if (input.wasPressed(Button::K4)) {
        if (state == State::PLAYING && xpEarned > 0) {
            progression.addXp(xpEarned);
            storage.save(petData);
        }

        return true;
    }

    if (state == State::GAME_OVER) {
        if (input.wasPressed(Button::K3)) resetGame();
        return false;
    }

    if (input.wasPressed(Button::K1)) moveFishUp();
    if (input.wasPressed(Button::K2)) moveFishDown();

    updatePlaying();

    return false;
}

void MiniGameScreen::draw() {
    if (state == State::GAME_OVER) drawGameOver();
    else drawPlaying();
}

void MiniGameScreen::resetGame() {
    state = State::PLAYING;

    fishY = GAME_TOP + ((GAME_BOTTOM - GAME_TOP - FRAME_MINIGAME_FISH.height) / 2);

    score = 0;
    xpEarned = 0;

    moveInterval = START_MOVE_INTERVAL;
    lastMoveAt = millis();

    for (uint8_t i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i] = Obstacle();
        spawnObstacle(i);
        obstacles[i].x += i * 45;
    }

    pet.activity();
}

void MiniGameScreen::updatePlaying() {
    if (millis() - lastMoveAt < moveInterval) return;

    lastMoveAt = millis();

    updateObstacles();
}

void MiniGameScreen::moveFishUp() {
    fishY -= FISH_STEP;

    if (fishY < GAME_TOP + 1) fishY = GAME_TOP + 1;

    pet.activity();
}

void MiniGameScreen::moveFishDown() {
    fishY += FISH_STEP;

    const int16_t maxY = GAME_BOTTOM - FRAME_MINIGAME_FISH.height;
    if (fishY > maxY) fishY = maxY;

    pet.activity();
}

void MiniGameScreen::updateObstacles() {
    for (uint8_t i = 0; i < MAX_OBSTACLES; i++) {
        Obstacle& obstacle = obstacles[i];

        if (!obstacle.active) continue;

        obstacle.x--;

        if (checkCollision(obstacle)) {
            gameOver();
            return;
        }

        const Bitmap& sprite = getObstacleSprite(obstacle.type);

        if (!obstacle.scored && obstacle.x + sprite.width < FISH_X) {
            obstacle.scored = true;

            score++;
            xpEarned++;

            increaseSpeed();
        }

        if (obstacle.x + sprite.width < 0) spawnObstacle(i);
    }
}

void MiniGameScreen::spawnObstacle(uint8_t index) {
    int16_t furthestX = SCREEN_W;

    for (uint8_t i = 0; i < MAX_OBSTACLES; i++) {
        if (i == index || !obstacles[i].active) continue;
        if (obstacles[i].x > furthestX) furthestX = obstacles[i].x;
    }

    Obstacle& obstacle = obstacles[index];

    obstacle.type = random(OBSTACLE_TYPE_COUNT);

    const Bitmap& sprite = getObstacleSprite(obstacle.type);

    obstacle.x = furthestX + random(SPAWN_MIN_DISTANCE, SPAWN_MAX_DISTANCE + 1);

    const int16_t fishCenterY = fishY + (FRAME_MINIGAME_FISH.height / 2);
    obstacle.y = fishCenterY - (sprite.height / 2);

    const int16_t minY = GAME_TOP + 1;
    const int16_t maxY = GAME_BOTTOM - sprite.height;

    if (obstacle.y < minY) obstacle.y = minY;
    if (obstacle.y > maxY) obstacle.y = maxY;

    obstacle.active = true;
    obstacle.scored = false;
}

void MiniGameScreen::increaseSpeed() {
    const unsigned long reduction = score * SPEED_STEP;

    if (reduction >= START_MOVE_INTERVAL - MIN_MOVE_INTERVAL) {
        moveInterval = MIN_MOVE_INTERVAL;
        return;
    }

    moveInterval = START_MOVE_INTERVAL - reduction;
}

bool MiniGameScreen::checkCollision(const Obstacle& obstacle) const {
    const Bitmap& sprite = getObstacleSprite(obstacle.type);

    const int16_t fishLeft = FISH_X + 1;
    const int16_t fishRight = FISH_X + FRAME_MINIGAME_FISH.width - 2;
    const int16_t fishTop = fishY + 1;
    const int16_t fishBottom = fishY + FRAME_MINIGAME_FISH.height - 2;

    const int16_t obstacleLeft = obstacle.x + 1;
    const int16_t obstacleRight = obstacle.x + sprite.width - 2;
    const int16_t obstacleTop = obstacle.y + 1;
    const int16_t obstacleBottom = obstacle.y + sprite.height - 2;

    return fishRight >= obstacleLeft &&
           fishLeft <= obstacleRight &&
           fishBottom >= obstacleTop &&
           fishTop <= obstacleBottom;
}

void MiniGameScreen::gameOver() {
    state = State::GAME_OVER;

    progression.addXp(xpEarned);

    if (score > petData.miniGameHighScore) petData.miniGameHighScore = score;

    storage.save(petData);

    Serial.print("[MINIGAME] Game over. Score: ");
    Serial.print(score);
    Serial.print(" | XP +");
    Serial.println(xpEarned);
}

void MiniGameScreen::drawPlaying() {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("SCORE ");
    display.print(score);

    display.setCursor(76, 0);
    display.print("HI ");
    display.print(petData.miniGameHighScore);

    display.drawFastHLine(0, 9, SCREEN_W, SSD1306_WHITE);

    drawFish();

    for (uint8_t i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].active) drawObstacle(obstacles[i]);
    }
}

void MiniGameScreen::drawGameOver() {
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(34, 4);
    display.print("GAME OVER");

    display.setCursor(22, 19);
    display.print("SCORE ");
    display.print(score);

    display.setCursor(22, 30);
    display.print("HIGH  ");
    display.print(petData.miniGameHighScore);

    display.setCursor(22, 41);
    display.print("XP   +");
    display.print(xpEarned);

    display.setCursor(0, 55);
    display.print("OK AGAIN");

    display.setCursor(92, 55);
    display.print("* BACK");
}

void MiniGameScreen::drawFish() {
    display.drawBitmap(
        FISH_X,
        fishY,
        FRAME_MINIGAME_FISH.data,
        FRAME_MINIGAME_FISH.width,
        FRAME_MINIGAME_FISH.height,
        SSD1306_WHITE
    );
}

void MiniGameScreen::drawObstacle(const Obstacle& obstacle) {
    const Bitmap& sprite = getObstacleSprite(obstacle.type);

    display.drawBitmap(
        obstacle.x,
        obstacle.y,
        sprite.data,
        sprite.width,
        sprite.height,
        SSD1306_WHITE
    );
}

const Bitmap& MiniGameScreen::getObstacleSprite(uint8_t type) const {
    switch (type) {
        case 0: return FRAME_MINIGAME_OBSTACLE; //FRAME_MINIGAME_OBSTACLE1
        case 1: return FRAME_MINIGAME_OBSTACLE; //FRAME_MINIGAME_OBSTACLE2
        case 2: return FRAME_MINIGAME_OBSTACLE; //FRAME_MINIGAME_OBSTACLE3
        case 3: return FRAME_MINIGAME_OBSTACLE; //FRAME_MINIGAME_OBSTACLE4
        case 4: return FRAME_MINIGAME_OBSTACLE; //FRAME_MINIGAME_OBSTACLE5
        default: return FRAME_MINIGAME_OBSTACLE;
    }
}