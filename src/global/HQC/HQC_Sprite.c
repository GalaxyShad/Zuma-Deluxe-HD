#include "../HQC.h"

typedef struct Sprite {
    HQC_Texture texture;
    irect_t textureRect;
} Sprite;


HQC_Sprite HQC_Sprite_Create(HQC_Texture tex, int rx, int ry, int rwidth, int rheight) {
    Sprite* sprite = HQC_Memory_Allocate(sizeof(*sprite));

    sprite->texture = tex;

    sprite->textureRect.x = rx;
    sprite->textureRect.y = ry;

    sprite->textureRect.width  = rwidth;
    sprite->textureRect.height = rheight;

    return sprite;
}


void HQC_Artist_DrawSprite(HQC_Sprite hsprite, float x, float y) {
    Sprite* sprite = (Sprite*)hsprite;

    HQC_Artist_DrawTextureRect(sprite->texture, x, y, sprite->textureRect);
}


void HQC_Sprite_Free(HQC_Sprite hsprite) {
    HQC_Memory_Free((Sprite*)hsprite);
}