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

irect_t HQC_Sprite_GetRect(HQC_Sprite hsprite) {
    Sprite* sprite = (Sprite*)hsprite;
    return sprite->textureRect;
}

HQC_Texture HQC_Sprite_GetTexture(HQC_Sprite hsprite) {
    Sprite* sprite = (Sprite*)hsprite;
    return sprite->texture;
}


void HQC_Artist_DrawSprite(HQC_Sprite hsprite, float x, float y) {
    Sprite* sprite = (Sprite*)hsprite;

    HQC_Artist_DrawTextureRect(sprite->texture, x, y, sprite->textureRect);
}


void HQC_Sprite_Free(HQC_Sprite hsprite) {
    HQC_Memory_Free((Sprite*)hsprite);
}