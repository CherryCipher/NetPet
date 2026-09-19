#include "animationmanager.h"

AnimationManager::AnimationManager(Adafruit_SSD1306& display) : display(display) {}

void AnimationManager::play(const Animation& animation, bool restart) {
    if (currentAnimation == &animation && playing && !restart) return;

    currentAnimation = &animation;
    currentFrame = 0;
    frameStartedAt = millis();
    playing = true;

    draw();
}

void AnimationManager::stop() {
    playing = false;
    currentAnimation = nullptr;
    currentFrame = 0;
}

void AnimationManager::update() {
    if (!playing || !currentAnimation || currentAnimation->frameCount == 0) return;

    unsigned long now = millis();
    const AnimationFrame& frame = currentAnimation->frames[currentFrame];

    if (now - frameStartedAt < frame.duration) return;

    nextFrame();
    frameStartedAt = now;
    draw();
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

void AnimationManager::nextFrame() {
    currentFrame++;

    if (currentFrame < currentAnimation->frameCount) return;

    if (currentAnimation->loop) {
        currentFrame = 0;
        return;
    }

    currentFrame = currentAnimation->frameCount - 1;
    playing = false;
}

bool AnimationManager::isPlaying() const {
    return playing;
}