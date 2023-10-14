#pragma once

#include "../global/HQC.h"
#include "ResourceStore.h"
#include "BallColors.h"
//#include "BallChain.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef void* HBullet;

BallColor Bullet_GetColor(HBullet hbullet);
v2f_t Bullet_GetPosition(HBullet hbullet);

void Bullet_SetPosition(HBullet hbullet, v2f_t position);
void Bullet_SetDirection(HBullet hbullet, float direction);

void Bullet_SetInsertion(HBullet hbullet, void* ball, bool isInsertingRight);

void* Bullet_GetInsertionBall(HBullet hbullet);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef void* HBulletList;

HBulletList BulletList_Create();
void BulletList_Update(HBulletList bulletList);
void BulletList_Draw(HBulletList bulletList);
void BulletList_Add(
	HBulletList bulletList, 
	BallColor bulletColor, v2f_t bulletPosition, float bulletSpd, float bulletDirection
);

HBullet BulletList_GetBullet(HBulletList bulletList, int index);

void BulletList_Free(HBulletList bulletList);


