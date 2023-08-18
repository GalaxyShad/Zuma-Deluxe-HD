#include "Bullets.h"
#include "BallChain.h"

#define ARR_SIZE		8
#define INSERT_TIME		15
#define INSERT_SPEED	0.5

typedef struct Bullet {
	v2f_t		pos;
	float		spd;
	float		direction;

	void*		insertionBallLeft;
	void*		insertionBallRight;

	bool		isInsertingRight;

	void*		insertionBallChain;

	int			insertTimer;

	HBulletList	bulletList;
	BallColor	color;

	HQC_Animation anim;
} Bullet;


BallColor Bullet_GetColor(HBullet hbullet) {
	return ((Bullet*)hbullet)->color;
}

v2f_t Bullet_GetPosition(HBullet hbullet) {
	return ((Bullet*)hbullet)->pos;
}

void Bullet_SetPosition(HBullet hbullet, v2f_t position) {
	Bullet* bullet = (Bullet*)hbullet;
	bullet->pos = position;
}

void Bullet_SetDirection(HBullet hbullet, float direction) {
	Bullet* bullet = (Bullet*)hbullet;
	bullet->direction = direction;
}

void Bullet_SetInsertion(HBullet hbullet, void* leftBall, void* rightBall, bool isInsertingRight) {
	if (!leftBall && !rightBall) return;
	
	Bullet* bullet = (Bullet*)hbullet;

	bullet->insertionBallLeft = leftBall;
	bullet->insertionBallRight = rightBall;

	bullet->isInsertingRight = isInsertingRight;

	bullet->insertionBallChain = (leftBall != NULL) ?
		Ball_GetChain(leftBall) : Ball_GetChain(rightBall);
}

void* Bullet_GetInsertionBallLeft(HBullet hbullet) {
	Bullet* bullet = (Bullet*)hbullet;

	return bullet->insertionBallLeft;
}

void* Bullet_GetInsertionBallRight(HBullet hbullet) {
	Bullet* bullet = (Bullet*)hbullet;

	return bullet->insertionBallRight;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct BulletList {
	Bullet* arr[ARR_SIZE];
} BulletList;


HBulletList BulletList_Create() {
	BulletList* list = HQC_Memory_Allocate(sizeof(*list));

	for (int i = 0; i < ARR_SIZE; list->arr[i] = NULL, i++);

	return list;
}

static void _BulletList_DestroyBullet(HBulletList bulletList, int index) {
	BulletList* bl = (BulletList*)bulletList;

	HQC_Memory_Free(bl->arr[index]);
	bl->arr[index] = NULL;
}

static Bullet* _Bullet_CollisionBullet(Bullet* bullet) {
	for (int i = 0; i < ARR_SIZE; i++) {
		Bullet* b = ((BulletList*)(bullet->bulletList))->arr[i];

		if (b == NULL || b == bullet)
			continue;

		float dist = HQC_PointDistance(bullet->pos.x, bullet->pos.y, b->pos.x, b->pos.y);

		if (dist <= 48)
			return b;
	}
	
	return NULL;
}

static void _Bullet_UpdateInserting(Bullet* bullet, int index) {
	if (!bullet->insertionBallChain)
		return;

	bullet->insertionBallLeft  = BallChain_HasBall(bullet->insertionBallChain, bullet->insertionBallLeft);
	bullet->insertionBallRight = BallChain_HasBall(bullet->insertionBallChain, bullet->insertionBallRight);

	if (!bullet->insertionBallLeft && !bullet->insertionBallRight) {
		_BulletList_DestroyBullet(bullet->bulletList, index);
		return;
	}

	

	Bullet* otherBullet = _Bullet_CollisionBullet(bullet);
	if (otherBullet) {
		otherBullet->insertTimer = 0;
	}

	HLevel hlvl = BallChain_GetLevel(bullet->insertionBallChain);

	
	
	float insertCurvePos = (!bullet->isInsertingRight && bullet->insertionBallLeft != NULL) ? 
		(Ball_GetPositionOnCurve(bullet->insertionBallLeft)  + 32) :
		(Ball_GetPositionOnCurve(bullet->insertionBallRight) - 32);

	v2f_t insertPos		 = Level_GetCurveCoords(hlvl, insertCurvePos);
	v2f_t insertDirPoint = Level_GetCurveCoords(hlvl, insertCurvePos + 1);

	bullet->direction = HQC_FAtan2(insertDirPoint.y - bullet->pos.y, insertDirPoint.x - bullet->pos.x);

	HQC_Animation_SetFrame(
		bullet->anim, 
		((int)insertCurvePos) % HQC_Animation_FramesCount(bullet->anim)
	);

	bullet->pos.x = HQC_Lerp(bullet->pos.x, insertPos.x, INSERT_SPEED);
	bullet->pos.y = HQC_Lerp(bullet->pos.y, insertPos.y, INSERT_SPEED);

	if (bullet->insertionBallRight != NULL && bullet->insertionBallLeft != NULL) {
		v2f_t ballRightPosCoords = Ball_GetPositionCoords(bullet->insertionBallRight);
		for (
			float dist = 0;

			(dist < 48) && (bullet->insertionBallRight != NULL) && 
			Ball_GetDistanceBetween(bullet->insertionBallLeft, bullet->insertionBallRight) < 64;

			Ball_MoveSubChainFrom(bullet->insertionBallRight, 1),
			ballRightPosCoords = Ball_GetPositionCoords(bullet->insertionBallRight),
			dist = HQC_PointDistance(bullet->pos.x, bullet->pos.y, ballRightPosCoords.x, ballRightPosCoords.y)
		);
	}

	if (bullet->insertTimer == 0) {
		HBall newBall;

		if (bullet->insertionBallLeft != NULL) {
			newBall = BallChain_InsertAfterBall(
				bullet->insertionBallChain,
				bullet->color,
				bullet->insertionBallLeft,
				insertCurvePos
			);
		}
		else {
			newBall = BallChain_InsertBeforeBall(
				Ball_GetChain(bullet->insertionBallRight),
				bullet->color,
				bullet->insertionBallRight,
				insertCurvePos
			);
		}

		Ball_BulletInsertDone(bullet->insertionBallLeft);
		Ball_BulletInsertDone(bullet->insertionBallRight);

		BallChain_ExplodeBalls(newBall);

		_BulletList_DestroyBullet(bullet->bulletList, index);

		return;
	}

	bullet->insertTimer--;
}


static void _Bullet_Update(Bullet* bullet, int index) {
	if (bullet == NULL) return;

	if (bullet->insertionBallLeft != NULL || bullet->insertionBallRight != NULL) {
		_Bullet_UpdateInserting(bullet, index);
		return;
	}

	bullet->pos.x += bullet->spd * HQC_FCos(bullet->direction);
	bullet->pos.y += bullet->spd * HQC_FSin(bullet->direction);

	if (bullet->pos.x < 0 || bullet->pos.x > 1280 || bullet->pos.y < 0 || bullet->pos.y > 720) {
		_BulletList_DestroyBullet(bullet->bulletList, index);
	}
}



void BulletList_Update(HBulletList bulletList) {
	BulletList* bl = (BulletList*)bulletList;

	for (int i = 0; i < ARR_SIZE; i++) {
		Bullet* bullet = bl->arr[i];

		if (bullet == NULL)
			continue;

		_Bullet_Update(bullet, i);
	}
}

#include <corecrt_math_defines.h>
void BulletList_Draw(HBulletList bulletList) {
	BulletList* bl = (BulletList*)bulletList;

	for (int i = 0; i < ARR_SIZE; i++) {
		Bullet* bullet = bl->arr[i];

		if (bullet == NULL)
			continue;

		HQC_Artist_DrawSetAngle(bullet->direction - M_PI_2);
		HQC_Artist_DrawAnimation(bullet->anim, (bullet->pos).x, (bullet->pos).y);
		HQC_Artist_DrawSetAngle(0);
	}
}


void BulletList_Add(
	HBulletList bulletList, BallColor bulletColor, v2f_t bulletPosition, float bulletSpd, float bulletDirection
) {
	BulletList* bl = (BulletList*)bulletList;

	for (int i = 0; i < ARR_SIZE; i++) {
		if (bl->arr[i] != NULL)
			continue;

		Bullet* bullet = HQC_Memory_Allocate(sizeof * bullet);

		bullet->pos				= bulletPosition;
		bullet->color			= bulletColor;
		bullet->direction		= bulletDirection;
		bullet->spd				= bulletSpd;

		bullet->insertionBallLeft  = NULL;
		bullet->insertionBallRight = NULL;

		bullet->isInsertingRight = false;

		bullet->insertTimer		= INSERT_TIME;
		bullet->bulletList		= bl;

		bullet->anim = HQC_Animation_Clone(Store_GetAnimationByID(ANIM_BALL_BLUE + bullet->color));

		bl->arr[i] = bullet;

		break;
	}
}


HBullet BulletList_GetBullet(HBulletList bulletList, int index) {
	if (index < 0 || index >= ARR_SIZE) {
		HQC_RaiseErrorHeaderFormat(
			"BulletList_GetBullet(HBulletList bulletList, int index)",
			"Index %d out of range [0..%d]", index, ARR_SIZE
		);
		return NULL;
	}
	
	BulletList* bl = (BulletList*)bulletList;
	
	return bl->arr[index];
}


void BulletList_Free(HBulletList bulletList) {
	
}
