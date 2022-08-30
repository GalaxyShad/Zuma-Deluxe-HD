#include "Animation.h"

int Animation_Init(Animation* anim, int textureID, SDL_Rect framePos) {
    if (!anim)
        return 0;

    anim->textureID = textureID;
    anim->framePos = framePos;

    anim->frame = 0;
    anim->frameDir = 1;
    anim->spd = 0;
    anim->startFrame = 0;
    anim->endFrame = 0;
    anim->angle = 0;
    anim->scale = 1;

    anim->color.a = 255;
    anim->color.r = 255;
    anim->color.g = 255;
    anim->color.b = 255;

    return 1;
}


void Animation_Set(Animation* anim, int startFrame, int endFrame, float spd) {
    if (!anim)
        return;

    anim->spd = spd;
    anim->startFrame = startFrame;
    anim->endFrame = endFrame;

    if (anim->frame < anim->startFrame ||
        anim->frame > anim->endFrame)
            anim->frame = anim->startFrame;

}

void Animation_SetFrame(Animation* anim, int frame) {
    if (!anim)
        return;

    anim->frame = frame;
}

void Animation_SetAngle(Animation* anim, float angle) {
    if (!anim)
        return;

    anim->angle = angle;
}


void Animation_Draw(Animation* anim, float x, float y) {
    if (!anim)
        return;

    anim->frame += anim->spd;
    if (anim->frame >= anim->endFrame && anim->spd != 0)
        anim->frame = anim->startFrame;

    if (anim->frame < anim->startFrame && anim->spd != 0)
        anim->frame = anim->endFrame-1;

    SDL_Texture* tex = Engine_GetTextureSDL(anim->textureID);
    SDL_Rect frame = anim->framePos;
    frame.y += (anim->framePos.h * (int)anim->frame) * anim->frameDir;

    SDL_FRect dRect;
    dRect.x = x - (anim->framePos.w * anim->scale)/2;
    dRect.y = y - (anim->framePos.h * anim->scale)/2;
    dRect.w = anim->framePos.w * anim->scale;
    dRect.h = anim->framePos.h * anim->scale;

    SDL_SetTextureColorMod(
        tex, 
        anim->color.r,
        anim->color.g,
        anim->color.b
    );

    SDL_SetTextureAlphaMod(
        tex, 
        anim->color.a
    );
    
    SDL_RenderCopyExF(
        engine.render,
        tex,
        &frame,
        &dRect,
        anim->angle,
        NULL,
        SDL_FLIP_NONE
    );
}