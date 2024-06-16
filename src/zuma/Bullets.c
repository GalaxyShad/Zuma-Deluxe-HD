#include <math.h>
#include "Bullets.h"
#include "BallChain.h"

#define ARR_SIZE		8
#define INSERT_TIME		15
#define INSERT_SPEED	0.5

typedef struct Bullet {
	v2f_t		pos;
	float		spd;
	float		direction;

	void*		insertionBall;
	bool		isInsertionBallOnRight;

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

void Bullet_SetInsertion(HBullet hbullet, void* ball, bool isInsertingRight) {
	if (!ball) return;
	
	Bullet* bullet = (Bullet*)hbullet;

	bullet->insertionBall = ball;
	bullet->insertionBallChain = Ball_GetChain(ball);
	bullet->isInsertionBallOnRight = isInsertingRight;
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

void* Bullet_GetInsertionBall(HBullet hbullet) {
	Bullet* bil = (Bullet*)hbullet;

	return bil->insertionBall;
}

static void _Bullet_UpdateInserting(Bullet* bullet, int index) {
	if (!bullet->insertionBallChain)
		return;


	// Handle unexisting chain and collision with other bullet
	bullet->insertionBall = BallChain_HasBall(bullet->insertionBallChain, bullet->insertionBall);

	if (!bullet->insertionBall) {
		_BulletList_DestroyBullet(bullet->bulletList, index);
		return;
	}

	Bullet* otherBullet = _Bullet_CollisionBullet(bullet);
	if (otherBullet) {
		otherBullet->insertTimer = 0;
	}


	// Detecting insert position
	HLevel hlvl = BallChain_GetLevel(bullet->insertionBallChain);

	float insertCurvePos = (bullet->isInsertionBallOnRight) ? 
		(Ball_GetPositionOnCurve(bullet->insertionBall) + 32) :
		(Ball_GetPositionOnCurve(bullet->insertionBall) - 32);

	v2f_t insertPos		 = Level_GetCurveCoords(hlvl, insertCurvePos);
	v2f_t insertDirPoint = Level_GetCurveCoords(hlvl, insertCurvePos + 1);


	// Animation stuff
	bullet->direction = HQC_FAtan2(insertDirPoint.y - bullet->pos.y, insertDirPoint.x - bullet->pos.x);

	HQC_Animation_SetFrame(
		bullet->anim, 
		((int)insertCurvePos) % HQC_Animation_FramesCount(bullet->anim)
	);


	// Update position
	bullet->pos.x = HQC_Lerp(bullet->pos.x, insertPos.x, INSERT_SPEED);
	bullet->pos.y = HQC_Lerp(bullet->pos.y, insertPos.y, INSERT_SPEED);


	// Shift chain
	void* fromBall = (bullet->isInsertionBallOnRight) ? Ball_Next(bullet->insertionBall) : bullet->insertionBall;
	if (fromBall == NULL)
		fromBall = bullet->insertionBall;

	void* prevBall = Ball_Previous(fromBall);

	v2f_t ballRightPosCoords = Ball_GetPositionCoords(prevBall);

	for (
		float dist = 0;

		(dist < 48) && (bullet->insertionBall != NULL) &&
		(Ball_GetDistanceBetween(prevBall, fromBall) < 64);

		Ball_MoveSubChainFrom(fromBall, 1),
		ballRightPosCoords = Ball_GetPositionCoords(prevBall),
		dist = HQC_PointDistance(bullet->pos.x, bullet->pos.y, ballRightPosCoords.x, ballRightPosCoords.y)
	);


	// Insert bullet to chain after insert animation
	if (bullet->insertTimer == 0) {
		HBall newBall;

		if (bullet->isInsertionBallOnRight) {
			newBall = BallChain_InsertAfterBall(
				bullet->insertionBallChain,
				bullet->color,
				bullet->insertionBall,
				insertCurvePos
			);
		}
		else {
			newBall = BallChain_InsertBeforeBall(
				bullet->insertionBallChain,
				bullet->color,
				bullet->insertionBall,
				insertCurvePos
			);
		}

		Ball_BulletInsertDone(bullet->insertionBall);

		BallChain_ExplodeBalls(newBall);

		_BulletList_DestroyBullet(bullet->bulletList, index);

		return;
	}

	bullet->insertTimer--;
}


static void _Bullet_Update(Bullet* bullet, int index) {
	if (bullet == NULL) return;

	if (bullet->insertionBall != NULL) {
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

		bullet->insertionBall			= NULL;
		bullet->isInsertionBallOnRight	= false;

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
