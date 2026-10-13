#include "ethernetscreen.h"

EthernetScreen::EthernetScreen(
    Adafruit_SSD1306& display,
    EthernetManager& ethernet,
    AnimationManager& animator
)
    : display(display),
      ethernet(ethernet),
      animator(animator) {}

void EthernetScreen::update() {
    switch (state) {
        case State::CONNECTING:
            if (animator.isFinished()) finishConnecting();
            break;

        case State::EATING:
            if (animator.isFinished()) finishEating();
            break;

        case State::EAT_RESULT:
            if (millis() - stateStartedAt >= RESULT_DURATION) {
                if (levelUpPending) startLevelUp();
                else finishFlow();
            }
            break;

        case State::LEVEL_UP:
            if (animator.isFinished()) {
                stateStartedAt = millis();
                state = State::LEVEL_UP_RESULT;
            }
            break;

        case State::LEVEL_UP_RESULT:
            if (millis() - stateStartedAt >= LEVEL_RESULT_DURATION) finishFlow();
            break;

        case State::IDLE:
            break;
    }
}

void EthernetScreen::draw() {
    switch (state) {
        case State::CONNECTING:
        case State::EATING:
            animator.draw();
            break;

        case State::EAT_RESULT:
            drawEatResult();
            break;

        case State::LEVEL_UP:
            drawLevelUp();
            break;

        case State::LEVEL_UP_RESULT:
            drawLevelUpResult();
            break;

        case State::IDLE:
            break;
    }
}

void EthernetScreen::startConnecting() {
    if (state != State::IDLE) return;

    Serial.println("[ETH SCREEN] Playing connect animation.");

    animator.play(ANIMATION_CONNECT_ETH, true);
    state = State::CONNECTING;
}

void EthernetScreen::startEating(const EatResult& result, const String& gatewayMAC, bool levelUp, uint16_t level) {
    this->result = result;
    this->gatewayMAC = gatewayMAC;
    this->levelUpPending = levelUp;
    this->level = level;

    Serial.println("[ETH SCREEN] Playing eat animation.");

    animator.play(ANIMATION_EAT_ETH, true);
    state = State::EATING;
}

bool EthernetScreen::isActive() const {
    return state != State::IDLE;
}

bool EthernetScreen::isConnecting() const {
    return state == State::CONNECTING;
}

void EthernetScreen::finishConnecting() {
    Serial.println("[ETH SCREEN] Connect animation finished.");

    state = State::IDLE;
}

void EthernetScreen::finishEating() {
    Serial.println("[ETH SCREEN] Eat animation finished.");

    stateStartedAt = millis();
    state = State::EAT_RESULT;
}

void EthernetScreen::startLevelUp() {
    Serial.print("[ETH SCREEN] Starting level-up sequence: LV ");
    Serial.println(level);

    animator.play(ANIMATION_LEVEL_UP, true);

    stateStartedAt = millis();
    state = State::LEVEL_UP;
}

void EthernetScreen::finishFlow() {
    levelUpPending = false;
    state = State::IDLE;

    Serial.println("[ETH SCREEN] Ethernet flow finished.");
}

void EthernetScreen::drawEatResult() {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("ETHERNET EATEN!");

    display.drawFastHLine(0, 11, 128, SSD1306_WHITE);

    display.setCursor(0, 18);
    display.print(gatewayMAC);

    display.setCursor(0, 32);
    display.print("+");
    display.print(result.energyGained);
    display.print(" ENERGY");

    display.setCursor(0, 43);
    display.print("+");
    display.print(result.finalXp);
    display.print(" XP");

    display.setCursor(75, 43);
    display.print("x");
    display.print(result.multiplier, 1);

    display.setCursor(0, 55);
    display.print("GATEWAY DISCOVERED");
}

void EthernetScreen::drawLevelUp() {
    animator.draw();
}

void EthernetScreen::drawLevelUpResult() {
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(34, 10);
    display.print("LEVEL UP!");

    display.setTextSize(2);
    display.setCursor(34, 32);
    display.print("LV ");
    display.print(level);
}