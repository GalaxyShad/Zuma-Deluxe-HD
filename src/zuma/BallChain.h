#pragma once

#include "Level.h"
#include "Bullets.h"
#include "BallColors.h"

typedef void* HBall;



float Ball_GetPositionOnCurve(HBall hball);
v2f_t Ball_GetPositionCoords(HBall ball);
void  Ball_MoveSubChainFrom(HBall hball, float value);
HBall Ball_Next(HBall hball);
void  Ball_BulletInsertDone(HBall hball);
float Ball_GetDistanceBetween(HBall hballLeft, HBall hballRight);

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

typedef void* HBallChain;

HBallChain Ball_GetChain(HBall ball);

HBall BallChain_HasBall(HBallChain hchain, HBall hball);

HBall BallChain_ExplodeBalls(HBall hstartBall);

HBallChain BallChain_Create(HLevel level, HBulletList bulletList);
HLevel BallChain_GetLevel(HBallChain hchain);
void BallChain_AddToStart(HBallChain hchain, BallColor color);

HBall BallChain_InsertBeforeBall(HBallChain hchain, BallColor color, HBall nextBall, float pos);
HBall BallChain_InsertAfterBall(HBallChain hchain, BallColor color, HBall prevBall, float pos);

void BallChain_Update(HBallChain hchain);
void BallChain_Draw(HBallChain hchain);