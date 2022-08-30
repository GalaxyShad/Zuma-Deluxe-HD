#ifndef ANIMATION_H
#define ANIMATION_H

#include "Engine.h"

typedef struct _Animation {
    int textureID;

    SDL_Rect framePos;
    float frame;
    float spd;
    int startFrame;
    int endFrame;
    int frameDir;

    float scale;
    SDL_Color color;

    float angle;
} Animation;

int Animation_Init(Animation* anim, int textureID, SDL_Rect framePos);
void Animation_Set(Animation* anim, int startFrame, int endFrame, float spd);
void Animation_SetFrame(Animation* anim, int frame);
void Animation_SetAngle(Animation* anim, float angle);
void Animation_Draw(Animation* anim, float x, float y);

#endif