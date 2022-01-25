#ifndef FROG_H
#define FROG_H

#include <Engine.h>
#include <Level.h>
#include <Enums.h>
#include <Animation.h>
#include <math.h>

#define FROG_CUR_BALL_PAD 35
#define FROG_NEXT_BALL_PAD 40
#define FROG_SHIFT_ON_SHOOT 10
#define FROG_TONGUE_SHIFT_ON_SHOOT 40

#define FROG_REVERT_SPEED 1
#define FROG_BLINK_SPEED 0.25
#define FROG_TOUNGUE_PAD 25

typedef struct _Frog {
	float x, y;
	char nextColor, color;
	float shift, tongueShift;
	double forwardX;
	double forwardY;
	double rotation;
	bool isShooted;
	Animation eyesAnim;
} Frog;

void Frog_Init(Frog*, float, float);
void Frog_Rotate(Frog*, int, int);
void Frog_Draw(Frog*, bool);

#endif