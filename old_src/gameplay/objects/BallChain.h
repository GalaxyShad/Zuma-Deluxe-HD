#ifndef BALL_H
#define BALL_H

#include <malloc.h>
#include <math.h>

#include "../../global/Enums.h"
#include "../../global/Animation.h"

#include "../Level.h"

#include "FX.h"

#define BALLS_CHAIN_PAD 			32.0f
#define BALLS_CHAIN_PAD_ROUGH 		BALLS_CHAIN_PAD+2.0f

#define BALLCHAIN_MAX_LEN 			256
#define BALL_SPEEDUP_ON_COLLIDE 	6
#define BALL_INSERTION_SPD 			4
#define BALL_EXPLODE_SPEED 			0.5

#define ROLLING_TO_PIT_SPEED 		32

#define BALL_ACC 					0.05
#define BALL_FRC 					0.04
#define BALL_DECC 					0.4
#define BALL_WEIGHT_RATIO 			2.5

#define BALL_MAX_BACK_SPEED 		-10

#define BALLCHAIN_BLINK_TIME 0.5
#define BALLCHAIN_BLINK_SPEED 4

#define RAD_TO_DEG 180/M_PI


typedef struct _Ball {
	char color, bonus;
	bool goBack, isInserted, isExploding, 
		isSingle, inTunnel, drawPrority, startAnim;
	float pos, x, y, dx, dy, spd;
	int combo;
	double ang;
	Animation anim;
} Ball;

typedef struct _BallChain {
	Ball balls[BALLCHAIN_MAX_LEN];
	int len;
	float speed;
	float startX, startY;
	int chainBonus, maxChainBonus;
	int maxCombo, totalCombos;
	int ballColors;
	bool colorInChain[6], isGenerating, isEndReached, isGlowing;
	clock_t glowTime;
	Animation ballShadow;
} BallChain;

void Ball_Init(Ball*, float, float, float, char, char, char);
void Ball_InitAnim(Ball*);
void Ball_Copy(Ball*, Ball*);
void Ball_Draw(Ball*);

void BallChain_Init(BallChain*, Level*, LevelSettings*);
void BallChain_Append(BallChain*, Level*, LevelSettings*);
void BallChain_FindSubChain(BallChain*, int, int*, int*);
void BallChain_Destroy(BallChain*, int, int);
void BallChain_Insert(BallChain*, int, char, bool);
int BallChain_GetLastMovingBall(BallChain*);

char BallChain_CollidesFront(BallChain*, int, int);
char BallChain_CollidesBack(BallChain*, int, int);

void BallChain_Update(BallChain*, CurveDot*, int, int*, Messages*);
void BallChain_Draw(BallChain*, bool);
void BallChain_UpdateColorInChain(BallChain* chain);
void BallChain_ExplodeBalls(BallChain*, int, int);
void BallChain_UpdateCombos(BallChain*);


#endif