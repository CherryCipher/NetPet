#include "petidle.h"

PetIdle::PetIdle(Adafruit_SSD1306& display) : display(display) {}

void PetIdle::begin() {
    sleeping = false;

    y = WORLD_TOP + ((WORLD_BOTTOM - WORLD_TOP - FISH_HEIGHT) / 2);

    for (Bubble& bubble : bubbles) bubble.active = false;

    resetFromOutside();

    lastMove = millis();
    lastBubble = millis();
    nextDirectionChange = millis() + random(2500, 6000);
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

    const Bitmap& fish = getFishBitmap();

    display.drawBitmap(
        x,
        y,
        fish.data,
        fish.width,
        fish.height,
        SSD1306_WHITE
    );

    if (sleeping) {
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
    lastMove = millis();
    lastBubble = millis();
    nextDirectionChange = millis() + random(2500, 6000);
}

void PetIdle::updateFish() {
    const unsigned long now = millis();

    if (now - lastMove < MOVE_INTERVAL) return;
    lastMove = now;

    x += direction;

    if (direction > 0 && x > SCREEN_WIDTH) {
        direction = -1;
        x = SCREEN_WIDTH;
    } else if (direction < 0 && x < -FISH_WIDTH) {
        direction = 1;
        x = -FISH_WIDTH;
    }

    if (now >= nextDirectionChange && x >= 0 && x <= SCREEN_WIDTH - FISH_WIDTH) {
        chooseDirection();
        nextDirectionChange = now + random(2500, 6000);
    }

    if (now - lastBubble >= BUBBLE_INTERVAL) {
        spawnBubble();
        lastBubble = now;
    }
}

void PetIdle::updateBubbles() {
    static unsigned long lastUpdate = 0;

    const unsigned long now = millis();

    if (now - lastUpdate < 100) return;
    lastUpdate = now;

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

void PetIdle::chooseDirection() {
    direction = random(0, 2) == 0 ? -1 : 1;
}

void PetIdle::resetFromOutside() {
    direction = random(0, 2) == 0 ? -1 : 1;

    if (direction > 0) x = -FISH_WIDTH;
    else x = SCREEN_WIDTH;
}

const Bitmap& PetIdle::getFishBitmap() const {
    if (sleeping) return direction > 0 ? FRAME_FISH_SICK_RIGHT : FRAME_FISH_SICK_LEFT;
    return direction > 0 ? FRAME_FISH_RIGHT : FRAME_FISH_LEFT;
}