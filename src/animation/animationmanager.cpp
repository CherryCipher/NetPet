#include "animationmanager.h"

AnimationManager::AnimationManager(Adafruit_SSD1306& display) : display(display) {}

void AnimationManager::play(const Animation& animation, bool restart) {
    if (currentAnimation == &animation && playing && !restart) return;

    currentAnimation = &animation;
    currentFrame = 0;
    frameStartedAt = millis();
    playing = true;
    finished = false;

    draw();
}

void AnimationManager::stop() {
    playing = false;
    finished = false;
    currentAnimation = nullptr;
    currentFrame = 0;
}

void AnimationManager::update() {
    if (!playing || !currentAnimation || currentAnimation->frameCount == 0) return;

    unsigned long now = millis();
    const AnimationFrame& frame = currentAnimation->frames[currentFrame];

    if (now - frameStartedAt < frame.duration) return;

    if (currentFrame + 1 < currentAnimation->frameCount) {
        currentFrame++;
        frameStartedAt = now;
        draw();
        return;
    }

    if (currentAnimation->loop) {
        currentFrame = 0;
        frameStartedAt = now;
        draw();
        return;
    }

    playing = false;
    finished = true;
}

void AnimationManager::draw() {
    if (!playing || !currentAnimation) return;

    display.fillRect(FACE_X, FACE_Y, FACE_WIDTH, FACE_HEIGHT, SSD1306_BLACK);

    const AnimationFrame& frame = currentAnimation->frames[currentFrame];

    display.drawBitmap(
        FACE_X,
        FACE_Y,
        frame.bitmap,
        FACE_WIDTH,
        FACE_HEIGHT,
        SSD1306_WHITE
    );

    display.display();
}


bool AnimationManager::isPlaying() const {
    return playing;
}

bool AnimationManager::isFinished() const {
    return finished;
}