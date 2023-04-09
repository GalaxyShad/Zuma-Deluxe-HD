#include "Game.h"

#include "Level.h"
#include "Frog.h"
#include "ResourceStore.h"

struct {
    HFrog  frog;
    HLevel level;
} game;


void Game_Start() {
    LevelSettings levelSettings;
    levelSettings.id = "some settings";

    LevelGraphics* levelGx = HQC_Memory_Allocate(sizeof(*levelGx));
    levelGx->dispName            = "When Spirals Attack";
    levelGx->coinsPosList        = HQC_Container_CreateVector(sizeof(v2f_t));
    levelGx->frogPos.x           = (242.0f + 104) * 1.5f;
    levelGx->frogPos.y           = 248.0f * 1.5f;
    levelGx->id                  = "tiltspiral";
    levelGx->textureFile         = "levels/tiltspiral/tiltspiral.jpg";
    levelGx->textureTopLayerFile = NULL;
    levelGx->curveAFile          = "levels/tiltspiral/tiltspiral.dat";
    levelGx->curveBFile          = NULL;

    game.level = Level_Load(&levelSettings, levelGx);

    game.frog = Frog_Create(levelGx->frogPos.x, levelGx->frogPos.y);
}


void Game_Update() {
    Frog_Update(game.frog);
}


void Game_Draw() {
    float cx = 1280 / 2;
    float cy = 720  / 2;

    Level_Draw(game.level, 1280 / 2, 720 / 2);       

    Frog_Draw(game.frog);

    for (int i = 0; i < 11; i++) {
        HQC_Animation_Tick(Store_GetAnimationByID(i));
        HQC_Artist_DrawAnimation(Store_GetAnimationByID(i), 78 + 64*i, 78);
    }

    HQC_Artist_DrawTextShadow(Store_GetFontByID(FONT_CANCUN_12), 
        "Hello", 256, 256);
    
    HQC_Artist_SetColorHex(0xffb347);
    HQC_Artist_DrawTextShadow(Store_GetFontByID(FONT_NATIVE_ALIEN_48), 
        Level_GetDisplayName(game.level), cx, 600);

    HQC_Artist_SetColorHex(C_GREEN);
    HQC_Artist_DrawTextShadow(Store_GetFontByID(FONT_CANCUN_8), 
        "Myau Its Working?", 256, 400);

    HQC_Artist_SetColorHex(C_WHITE);

    HQC_Artist_DrawSprite(Store_GetSpriteByID(SPR_MENU_HEAD), 500, 500);

    HQC_Artist_DrawSprite(Store_GetSpriteByID(SPR_GAME_HUD_BORDER), cx, cy);
    
    // HQC_Artist_DrawAnimation(Store_GetAnimationByID(0), 78, 78);
}