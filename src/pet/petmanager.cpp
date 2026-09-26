#include "petidle.h"

PetIdle::PetIdle(Adafruit_SSD1306& display) : display(display) {}

void PetIdle::begin() {
    sleeping = false;
    sick = false;

    y = WORLD_TOP + ((WORLD_BOTTOM - WORLD_TOP - FISH_HEIGHT) / 2);

    for (Bubble& bubble : bubbles) bubble.active = false;

    resetFromOutside();

    const unsigned long now = millis();

    lastMove = now;
    lastBubble = now;
    lastBubbleMove = now;

    idleState = IdleState::SWIMMING;
    nextPause = now + random(2500, 6000);
}

void PetIdle::update() {
    updateBubbles();

    if (sleeping) return;

    updateFish();
}

void PetIdle::draw() const {
    for (const Bubble& bubble : bubbles) {
        if (!bubble.active) continue;
        display.drawPixel(bubble.x, bubble.y, SSD1306_WHITE);
    }

    if (idleState != IdleState::OFFSCREEN) {
        const Bitmap& fish = getFishBitmap();

        display.drawBitmap(
            x,
            y,
            fish.data,
            fish.width,
            fish.height,
            SSD1306_WHITE
        );
    }

    if (sleeping && idleState != IdleState::OFFSCREEN) {
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);

        display.setCursor(x + FISH_WIDTH - 2, y);
        display.print("z");

        display.setCursor(x + FISH_WIDTH + 4, y - 7);
        display.print("Z");
    }
}

void PetIdle::sleep() {
    sleeping = true;
}

void PetIdle::wake() {
    if (!sleeping) return;

    sleeping = false;

    const unsigned long now = millis();

    lastMove = now;
    lastBubble = now;

    if (idleState == IdleState::PAUSED) startSwimming();
}

void PetIdle::setSick(bool sick) {
    this->sick = sick;
}

void PetIdle::updateFish() {
    const unsigned long now = millis();

    if (idleState == IdleState::PAUSED) {
        if (now >= stateUntil) startSwimming();
        return;
    }

    if (idleState == IdleState::OFFSCREEN) {
        if (now >= stateUntil) {
            resetFromOutside();
            startSwimming();
        }

        return;
    }

    if (now - lastMove < MOVE_INTERVAL) return;
    lastMove = now;

    x += direction;

    if (isFullyOffscreen()) {
        startOffscreen();
        return;
    }

    if (isFullyVisible() && now >= nextPause) {
        startPause();
        return;
    }

    if (now - lastBubble >= BUBBLE_INTERVAL) {
        spawnBubble();
        lastBubble = now;
    }
}

void PetIdle::updateBubbles() {
    const unsigned long now = millis();

    if (now - lastBubbleMove < BUBBLE_MOVE_INTERVAL) return;
    lastBubbleMove = now;

    for (Bubble& bubble : bubbles) {
        if (!bubble.active) continue;

        bubble.y--;

        if (bubble.y < WORLD_TOP) bubble.active = false;
    }
}

void PetIdle::spawnBubble() {
    for (Bubble& bubble : bubbles) {
        if (bubble.active) continue;

        bubble.x = direction > 0 ? x : x + FISH_WIDTH;
        bubble.y = y + (FISH_HEIGHT / 2) + random(-4, 5);
        bubble.active = true;

        return;
    }
}

void PetIdle::startSwimming() {
    const unsigned long now = millis();

    idleState = IdleState::SWIMMING;
    lastMove = now;
    nextPause = now + random(2500, 6000);
}

void PetIdle::startPause() {
    idleState = IdleState::PAUSED;
    stateUntil = millis() + random(500, 2000);
}

void PetIdle::startOffscreen() {
    idleState = IdleState::OFFSCREEN;
    stateUntil = millis() + random(1000, 4000);
}

void PetIdle::resetFromOutside() {
    direction = random(0, 2) == 0 ? -1 : 1;

    if (direction > 0) x = -FISH_WIDTH;
    else x = SCREEN_WIDTH;
}

bool PetIdle::isFullyOffscreen() const {
    if (direction > 0) return x > SCREEN_WIDTH;
    return x < -FISH_WIDTH;
}

bool PetIdle::isFullyVisible() const {
    return x >= 0 && x <= SCREEN_WIDTH - FISH_WIDTH;
}

const Bitmap& PetIdle::getFishBitmap() const {
    if (sleeping) return direction > 0 ? FRAME_FISH_SLEEP_RIGHT : FRAME_FISH_SLEEP_LEFT;
    if (sick) return direction > 0 ? FRAME_FISH_SICK_RIGHT : FRAME_FISH_SICK_LEFT;

    return direction > 0 ? FRAME_FISH_RIGHT : FRAME_FISH_LEFT;
}