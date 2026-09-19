#include "animationmanager.h"

AnimationManager::AnimationManager(Adafruit_SSD1306& display) : display(display) {}

void AnimationManager::play(const Animation& animation, bool restart) {
    if (currentAnimation == &animation && playing && !restart) return;

    currentAnimation = &animation;
    currentFrame = 0;
    frameStartedAt = millis();
    playing = true;
    finished = false;
}

void AnimationManager::stop() {
    currentAnimation = nullptr;
    currentFrame = 0;
    frameStartedAt = 0;
    playing = false;
    finished = false;
}

void AnimationManager::update() {
    if (!playing || !currentAnimation || currentAnimation->frameCount == 0) return;

    const unsigned long now = millis();
    const AnimationFrame& frame = currentAnimation->frames[currentFrame];

    if (now - frameStartedAt < frame.duration) return;

    if (currentFrame + 1 < currentAnimation->frameCount) {
        currentFrame++;
        frameStartedAt = now;
        return;
    }

    if (currentAnimation->loop) {
        currentFrame = 0;
        frameStartedAt = now;
        return;
    }

    playing = false;
    finished = true;
}

void AnimationManager::draw() const {
    if (!currentAnimation || currentAnimation->frameCount == 0) return;

    const AnimationFrame& frame = currentAnimation->frames[currentFrame];

    display.drawBitmap(
        FACE_X,
        FACE_Y,
        frame.bitmap,
        FACE_WIDTH,
        FACE_HEIGHT,
        SSD1306_WHITE
    );
}

bool AnimationManager::isPlaying() const {
    return playing;
}

bool AnimationManager::isFinished() const {
    return finished;
}