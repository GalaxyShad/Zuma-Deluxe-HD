#ifndef BULLET_H
#define BULLET_H

#include <Level.h>
#include <Enums.h>
#include <Animation.h>
#include <BallChain.h>
#include <Frog.h>
#include <math.h>
#include <Engine.h>


#define BULLETS_ARR_MAX_LEN 32
#define BULLET_SPEED 15

// The square of the distance required for a collision
#define DISTANCE_TO_COLLIDE 2300
#define TO_DEGREES (180.0 / M_PI)

typedef struct _Bullet {
	char color, onScreen;
	float x, y, motionVecX, motionVecY;
	Animation anim;
} Bullet;

typedef struct _BulletsArr {
	Bullet bullets[BULLETS_ARR_MAX_LEN];
	int len;
} BulletsArr;

void Bullet_Init(Bullet*, float, float, float, float, char);
void Bullet_Update(Bullet*);
void Bullet_Draw(Bullet*);

void BulletsArr_CollideWithChainUpdate(BulletsArr*, BallChain*, SpiralDot*);
void BulletsArr_UpdateOnScreenStatus(BulletsArr*, int*);
void BulletsArr_AddBullet(BulletsArr*, Frog*);
void BulletsArr_Update(BulletsArr*);
void BulletsArr_Draw(BulletsArr*);

#endif
