#include "../HQC.h"

typedef struct Animation {
    HQC_VECTOR(HQC_Sprite)  frameList;
    float                   spd;
    float                   currentFrame;

    size_t                  frameCount;
} Animation;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

Animation* _Cast(HQC_Animation hanim) {
    return (Animation*)hanim;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

HQC_Animation HQC_Animation_Create() {
    Animation* anim = HQC_Memory_Allocate(sizeof(*anim));

    anim->frameList     = HQC_Container_CreateVector(sizeof(HQC_Sprite));
    anim->spd           = 1.f;
    anim->currentFrame  = 0.f;
    anim->frameCount    = 1;

    return anim;
}


void    HQC_Animation_AddFrame(HQC_Animation anim, HQC_Sprite frame) {
    Animation* animation = _Cast(anim);
    HQC_Container_VectorAdd(animation->frameList, &frame);
    animation->frameCount = (float)HQC_Container_VectorCount(animation->frameList);
}


void    HQC_Animation_SetFrame(HQC_Animation anim, int frame) {
    Animation* animation = _Cast(anim);
    animation->currentFrame = frame;
}


void    HQC_Animation_Tick(HQC_Animation anim) {
    Animation* animation = _Cast(anim);

    animation->currentFrame += animation->spd;

    if (animation->currentFrame >= animation->frameCount)
        animation->currentFrame -= animation->frameCount;

}


void    HQC_Artist_DrawAnimation(HQC_Animation anim, float x, float y) {
    Animation* animation = _Cast(anim);

    HQC_Sprite* spr = ((HQC_Sprite*)HQC_Container_VectorGet(
        animation->frameList, (int)animation->currentFrame));
    
    HQC_Artist_DrawSprite(*spr, x, y);
}


void    HQC_Animation_SetSpeed(HQC_Animation anim, float spd) {
    _Cast(anim)->spd = spd;
}


float   HQC_Animation_GetSpeed(HQC_Animation anim) {
    return _Cast(anim)->spd;
}


void    HQC_Animation_Free(HQC_Animation anim) {
    Animation* animation = _Cast(anim);
    
    HQC_Container_FreeVector(animation->frameList);
    HQC_Memory_Free(animation);

    anim = NULL;
}