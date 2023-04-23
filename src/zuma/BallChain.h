#pragma once

#include "Level.h"

typedef void* HBallChain;

typedef enum BallColor {
    BALL_BLUE,
    BALL_GREEN,
    BALL_YELLOW,
    BALL_RED,
    BALL_PURPLE,
    BALL_GRAY
} BallColor;


HBallChain BallChain_Create(HLevel level);
void BallChain_AddToStart(HBallChain hchain, BallColor color);
void BallChain_Update(HBallChain hchain);
void BallChain_Draw(HBallChain hchain);