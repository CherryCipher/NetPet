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

void AnimationManager::draw(int16_t x, int16_t y) const {
    if (!currentAnimation || currentAnimation->frameCount == 0) return;

    const AnimationFrame& frame = currentAnimation->frames[currentFrame];
    if (!frame.bitmap || !frame.bitmap->data) return;

    display.drawBitmap(
        x,
        y,
        frame.bitmap->data,
        frame.bitmap->width,
        frame.bitmap->height,
        SSD1306_WHITE
    );
}

bool AnimationManager::isPlaying() const {
    return playing;
}

bool AnimationManager::isFinished() const {
    return finished;
}