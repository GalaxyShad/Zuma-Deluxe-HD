#include <stdio.h>
#include "Statistics.h"

#include "FloatingText.h"

static int gapPoints_ = 0;
static int gapCount_ = 0;
static int ballsCount_ = 0;
static int chainCount_ = 0;
static int comboCount_ = 0;
static BallColor prevBallColor = BALL_NONE;
static BallColor curBallColor = BALL_NONE;

void Statistics_Init() {
    gapPoints_ = 0;
    gapCount_ = 0;
    ballsCount_ = 0;
    chainCount_ = 0;
    comboCount_ = 0;
    prevBallColor = BALL_NONE;
}

void Statistics_AddExplodedBalls(int explodedBalls, BallColor ballsColor) {
    ballsCount_ = explodedBalls;

    if (ballsColor == prevBallColor) {
        comboCount_++;
    } else {
        comboCount_ = 0;
    }

    prevBallColor = curBallColor;
    curBallColor = ballsColor;
}

void Statistics_BuildAndInstantiateFloatingText(float x, float y) {
    int points = ballsCount_ * 10 + gapPoints_ + comboCount_ * 100;

    bool isChainBonus = chainCount_ >= 5;
    if (isChainBonus && comboCount_ == 0) {
        points += 100 + 10 * (chainCount_ - 5);
    }

    char comboText[48] = {0};
    if (comboCount_ > 0) {
        snprintf(comboText, 48, "COMBO X%d\n", comboCount_+1);
    }

    char gapText[48] = {0};
    if (gapCount_ > 0) {
        snprintf(gapText, 48, "%sGAP BONUS\n", gapCount_ == 3 ? "TRIPLE" : gapCount_ == 2 ? "DOUBLE" : "");
    }

    char chainText[48] = {0};
    if (isChainBonus) {
        snprintf(chainText, 48, "CHAIN BONUS X%d\n", chainCount_);
    }

    FloatingTextFactory_Instantiate(
            x, y,
            0xFF00FF,
            "+%d\n%s%s%s", points, comboText, gapText, chainText
            );

    ballsCount_ = 0;
    gapCount_ = 0;
    gapPoints_ = 0;
}

void Statistics_IncrementChain() {
    chainCount_++;
}

void Statistics_BreakChain() {
    chainCount_ = 0;
}

void Statistics_AddBulletGap(float distance) {
    distance -= 52 * 1.5f;

    if (distance < 0) {
        distance = 0;
    }

    const float MAX_GAP_SIZE = 300 * 1.5f;

    float gapBonus = 500.0f;
    int gapBonusMultiplier = (int)(gapBonus * (MAX_GAP_SIZE - distance) / MAX_GAP_SIZE);

    if (gapBonusMultiplier < 10)
        gapBonusMultiplier = 10;

    if (gapCount_ > 1)
        gapBonusMultiplier *= 2;

    gapPoints_ += gapBonusMultiplier;
    gapCount_ += 1;
}


