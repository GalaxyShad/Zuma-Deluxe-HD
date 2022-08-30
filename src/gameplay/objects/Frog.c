#include "Frog.h"

#define TO_DEGREES (180.0 / M_PI)

void Frog_Init(Frog* frog, float x, float y) {
	frog->nextColor = 1;
	frog->color = 0;
	frog->x = x;
	frog->y = y;

	frog->forwardX = 0;
	frog->forwardY = 1;
	frog->isShooted = 0;
	frog->shift = 0;
	frog->tongueShift = 0;

	SDL_Rect animEyesRect = {0, 162, 162, 162};
	Animation_Init(&frog->eyesAnim, TEX_FROG, animEyesRect);
	Animation_Set(&frog->eyesAnim, 0, 4, 0);
}

void Frog_Rotate(Frog* frog, int mx, int my) {
	int w, h;

	double vecX = mx - frog->x;
	double vecY = my - frog->y;

	frog->rotation = (2 * M_PI - atan2(vecX, vecY)) * TO_DEGREES;

	double length = sqrt(vecX * vecX + vecY * vecY);
	vecX = vecX / length;
	vecY = vecY / length;

	frog->forwardX = vecX;
	frog->forwardY = vecY;
}

void Frog_Draw(Frog* frog, bool introEnd) {
	float frogX = frog->x;
	float frogY = frog->y;

	if (frog->isShooted) {
		frog->shift = FROG_SHIFT_ON_SHOOT;
		frog->tongueShift = FROG_TONGUE_SHIFT_ON_SHOOT;
	}

	if (frog->shift > 0) {
		frogX -= frog->forwardX * frog->shift;
		frogY -= frog->forwardY * frog->shift;
    	frog->shift -= FROG_REVERT_SPEED;
    }

    if (frog->tongueShift > 0)
    	frog->tongueShift -= FROG_REVERT_SPEED * (FROG_TONGUE_SHIFT_ON_SHOOT / FROG_SHIFT_ON_SHOOT);

	if (!introEnd)
		frog->tongueShift = 24;

    Animation anim;
    //Drawing Foundation
    SDL_Rect animFoundationRect = {162, 162, 162, 162};
	Animation_Init(&anim, TEX_FROG, animFoundationRect);
	Animation_SetAngle(&anim, frog->rotation);
	Animation_Draw(&anim, frogX, frogY);

    //Drawing BotBodyLayer
	SDL_Rect animBodyRect = {0, 0, 162, 162};
	Animation_Init(&anim, TEX_FROG, animBodyRect);
	Animation_SetAngle(&anim, frog->rotation);
	Animation_Draw(&anim, frogX, frogY);

	//Drawing Tongue
	SDL_Rect animTongueRect = {162, 0, 162, 162};
	Animation_Init(&anim, TEX_FROG, animTongueRect);
	Animation_SetAngle(&anim, frog->rotation);
	Animation_Draw(&anim, frogX + frog->forwardX*(FROG_TOUNGUE_PAD-frog->tongueShift),
		 frogY + frog->forwardY*(FROG_TOUNGUE_PAD - frog->tongueShift));

	if (introEnd) {
	//Drawing  currentBall
    SDL_Rect animBallRect = {48 * frog->color, 0, 48, 48};
	Animation_Init(&anim, TEX_GAME_OBJECTS, animBallRect);
	Animation_Set(&anim, 24, 47, 0);
	Animation_SetAngle(&anim, frog->rotation);
	Animation_Draw(&anim, frogX + frog->forwardX*(FROG_CUR_BALL_PAD-frog->tongueShift), 
		frogY + frog->forwardY*(FROG_CUR_BALL_PAD-frog->tongueShift));

	//Drawing  nextBall
    SDL_Rect animBallRect1 = {15 * frog->nextColor + 234, 633, 15, 15};
	Animation_Init(&anim, TEX_FROG, animBallRect1);
	Animation_Set(&anim, 0, 0, 0);
	Animation_SetAngle(&anim, frog->rotation);
	anim.scale = 1.6;
	if (!frog->isShooted)
		Animation_Draw(&anim, frogX - frog->forwardX*FROG_NEXT_BALL_PAD, frogY - frog->forwardY*FROG_NEXT_BALL_PAD);
	}

	//Drawing TopBodyLayer
	anim.scale = 1;
	if (frog->isShooted) {
		Animation_Set(&frog->eyesAnim, 0, 3, FROG_BLINK_SPEED);
	}

	Animation_SetAngle(&frog->eyesAnim, frog->rotation);
	Animation_Draw(&frog->eyesAnim, frogX, frogY);

	if (frog->eyesAnim.frame == 0) {
		Animation_Set(&frog->eyesAnim, 0, 3, 0);
		Animation_SetFrame(&frog->eyesAnim, 0);
	}

    frog->isShooted = 0;
}