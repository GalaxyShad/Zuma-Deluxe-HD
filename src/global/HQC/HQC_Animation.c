#include "../HQC.h"

typedef struct Animation {
    HQC_VECTOR(HQC_Sprite)  frameList;

    float                   spd;
    float                   currentFrame;

    bool                    isLooping;

    size_t                  frameCount;
} Animation;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

Animation* Cast__(HQC_Animation hanim) {
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
    anim->isLooping     = true;


    return anim;
}


void HQC_Animation_SetLooping(HQC_Animation anim, bool looping) {
    Animation* animation = Cast__(anim);

    animation->isLooping = looping;
}


bool HQC_Animation_IsEnded(HQC_Animation anim) {
    Animation* animation = Cast__(anim);

    if (animation->isLooping)
        return false;

    return (animation->currentFrame >= animation->frameCount - 1);
}

HQC_Animation HQC_Animation_Clone(HQC_Animation hsrc) {
    Animation* src = (Animation*)hsrc;
    Animation* dst = (Animation*)HQC_Animation_Create();

    for (int i = 0; i < HQC_Container_VectorCount(src->frameList); i++) {
        HQC_Container_VectorAdd(dst->frameList, HQC_Container_VectorGet(src->frameList, i));
    }

    dst->spd            = src->spd;
    dst->currentFrame   = src->currentFrame;
    dst->frameCount     = src->frameCount;
    dst->isLooping      = src->isLooping;

    return dst;
}


void    HQC_Animation_AddFrame(HQC_Animation anim, HQC_Sprite frame) {
    Animation* animation = Cast__(anim);
    HQC_Container_VectorAdd(animation->frameList, &frame);
    animation->frameCount = (float)HQC_Container_VectorCount(animation->frameList);
}


size_t HQC_Animation_FramesCount(HQC_Animation anim) {
    Animation* animation = Cast__(anim);
    return animation->frameCount;
}


void    HQC_Animation_SetFrame(HQC_Animation anim, int frame) {
    Animation* animation = Cast__(anim);
    animation->currentFrame = frame;
}


void    HQC_Animation_Tick(HQC_Animation anim) {
    Animation* animation = Cast__(anim);

    animation->currentFrame += animation->spd;

    if (animation->currentFrame >= animation->frameCount) {
        if (animation->isLooping)
            animation->currentFrame -= animation->frameCount;
        else
            animation->currentFrame = animation->frameCount - 1;
    }

}


void    HQC_Artist_DrawAnimation(HQC_Animation anim, float x, float y) {
    Animation* animation = Cast__(anim);

    HQC_Sprite* spr = ((HQC_Sprite*)HQC_Container_VectorGet(
        animation->frameList, (int)animation->currentFrame));
    
    HQC_Artist_DrawSprite(*spr, x, y);
}


void    HQC_Animation_SetSpeed(HQC_Animation anim, float spd) {
    Cast__(anim)->spd = spd;
}


float   HQC_Animation_GetSpeed(HQC_Animation anim) {
    return Cast__(anim)->spd;
}


void    HQC_Animation_Free(HQC_Animation anim) {
    Animation* animation = Cast__(anim);
    
    HQC_Container_FreeVector(animation->frameList);
    HQC_Memory_Free(animation);

    anim = NULL;
}