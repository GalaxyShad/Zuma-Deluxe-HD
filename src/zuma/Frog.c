#include "Frog.h"

#include "../global/HQC.h"

#include "ResourceStore.h"
#include "BallColors.h"

typedef struct Frog {
    HBulletList bulletList;

    HQC_Sprite spr;
    HQC_Sprite sprTongue;
    HQC_Sprite sprPlate;

    BallColor  ballColor;

    float angle;
    v2f_t pos;  
} Frog; 

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

static Frog* _Frog(HFrog hfrog) {
    return (Frog*)hfrog;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

HFrog Frog_Create(float x, float y, HBulletList bulletList) {
    Frog* frog = HQC_Memory_Allocate(sizeof(*frog));

    frog->angle         = 0.f;
    frog->spr           = Store_GetSpriteByID(SPR_FROG);
    frog->sprTongue     = Store_GetSpriteByID(SPR_FROG_TONGUE);
    frog->sprPlate      = Store_GetSpriteByID(SPR_FROG_PLATE);

    frog->bulletList    = bulletList;
    frog->ballColor = HQC_RandomRange(0, 4);

    frog->pos.x         = x;
    frog->pos.y         = y;

    return frog;
}

#define _USE_MATH_DEFINES
#include <math.h>
void Frog_Update(HFrog hfrog) {
    Frog* frog = _Frog(hfrog);

    v2i_t mousePos = HQC_Input_MouseGetPosition();

    frog->angle = atan2(mousePos.y - frog->pos.y, mousePos.x - frog->pos.x);

    static bool s_isMouseLeftPressed = false;

    if (HQC_Input_MouseLeftPressed() && !s_isMouseLeftPressed) {
        HQC_DJ_PlaySound(Store_GetSoundByID(SND_FIREBALL1));
        BulletList_Add(frog->bulletList, frog->ballColor, frog->pos, 16.f, frog->angle);
        frog->ballColor = HQC_RandomRange(0, 4);
        s_isMouseLeftPressed = true;
    }

    if (!HQC_Input_MouseLeftPressed())
        s_isMouseLeftPressed = false;
}


void Frog_Draw(HFrog hfrog) {
    Frog* frog = _Frog(hfrog);

    HQC_Artist_DrawSprite(frog->sprPlate, frog->pos.x, frog->pos.y);
    
    HQC_Artist_DrawSetAngle(frog->angle - M_PI_2);
    HQC_Artist_DrawSprite(frog->spr, frog->pos.x, frog->pos.y);
    HQC_Artist_DrawAnimation(Store_GetAnimationByID(ANIM_BALL_BLUE + frog->ballColor), frog->pos.x, frog->pos.y);
    HQC_Artist_DrawSetAngleDegrees(0);
    // HQC_Artist_DrawSprite(_Frog(hfrog)->sprTongue,  _Frog(hfrog)->pos.x, _Frog(hfrog)->pos.y);
}


void Frog_Destroy(HFrog hfrog) {
    HQC_Memory_Free(hfrog);
}