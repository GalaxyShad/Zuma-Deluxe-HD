#include "Frog.h"

#include "../global/HQC.h"

#include "ResourceStore.h"

typedef struct Frog {
    HQC_Sprite spr;
    HQC_Sprite sprTongue;
    HQC_Sprite sprPlate;

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

HFrog Frog_Create(float x, float y) {
    Frog* frog = HQC_Memory_Allocate(sizeof(*frog));

    frog->angle         = 0.f;
    frog->spr           = Store_GetSpriteByID(SPR_FROG);
    frog->sprTongue     = Store_GetSpriteByID(SPR_FROG_TONGUE);
    frog->sprPlate      = Store_GetSpriteByID(SPR_FROG_PLATE);

    frog->pos.x         = x;
    frog->pos.y         = y;

    return frog;
}

#define _USE_MATH_DEFINES
#include <math.h>
void Frog_Update(HFrog hfrog) {
    Frog* frog = _Frog(hfrog);

    v2i_t mousePos = HQC_Input_MouseGetPosition();
    // HQC_Log("%d %d", mousePos.x, mousePos.y);


    frog->angle = M_PI_2 + atan2(frog->pos.y - mousePos.y, frog->pos.x - mousePos.x);
}


void Frog_Draw(HFrog hfrog) {

    HQC_Artist_DrawSprite(_Frog(hfrog)->sprPlate,   _Frog(hfrog)->pos.x, _Frog(hfrog)->pos.y);
    
    HQC_Artist_DrawSetAngle(_Frog(hfrog)->angle);
    HQC_Artist_DrawSprite(_Frog(hfrog)->spr,        _Frog(hfrog)->pos.x, _Frog(hfrog)->pos.y);
    HQC_Artist_DrawSetAngleDegrees(0);
    // HQC_Artist_DrawSprite(_Frog(hfrog)->sprTongue,  _Frog(hfrog)->pos.x, _Frog(hfrog)->pos.y);
}


void Frog_Destroy(HFrog hfrog) {
    HQC_Memory_Free(hfrog);
}