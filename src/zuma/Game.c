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

    LevelGraphics levelGx;
    levelGx.dispName            = "Nice lol";
    levelGx.coinsPosList        = HQC_Container_CreateVector(sizeof(v2f_t));
    levelGx.frogPos.x           = 32;
    levelGx.frogPos.y           = 32;
    levelGx.id                  = "id nice lol";
    levelGx.textureFile         = "levels/spiral/spiral.jpg";
    levelGx.textureTopLayerFile = NULL;
    levelGx.curveAFile          = "levels/spiral/spiral.dat";
    levelGx.curveBFile          = NULL;

    game.level = Level_Load(&levelSettings, &levelGx);


    HQC_Log("ss -> %p", Store_GetTextureByID(TEX_FROG));

    game.frog = Frog_Create(1280 / 2, 720  / 2);
}


void Game_Update() {
    Frog_Update(game.frog);
}


void Game_Draw() {
    float cx = 1280 / 2;
    float cy = 720  / 2;

    Level_Draw(game.level, 1280 / 2, 720 / 2);       

    Frog_Draw(game.frog);

    for (int i = 0; i < 7; i++) {
        HQC_Animation_Tick(Store_GetAnimationByID(i));
        HQC_Artist_DrawAnimation(Store_GetAnimationByID(i), 78 + 64*i, 78+ 64*i);
    }

    HQC_Artist_DrawTextShadow(Store_GetFontByID(FONT_CANCUN_12), 
        "Hello", 256, 256);
    
    HQC_Artist_SetColorHex(C_CYAN);
    HQC_Artist_DrawTextShadow(Store_GetFontByID(FONT_NATIVE_ALIEN_48), 
        "Myau Its Working?", 256, 320);

    HQC_Artist_SetColorHex(C_GREEN);
    HQC_Artist_DrawText(Store_GetFontByID(FONT_CANCUN_8), 
        "Myau Its Working?", 256, 400);

    HQC_Artist_SetColorHex(C_WHITE);
    
    // HQC_Artist_DrawAnimation(Store_GetAnimationByID(0), 78, 78);
}