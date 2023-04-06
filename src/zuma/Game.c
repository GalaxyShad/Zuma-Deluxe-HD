#include "Game.h"

#include "Level.h"
#include "ResourceStore.h"

struct {
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
}


void Game_Update() {

}


void Game_Draw() {
    float cx = 1280 / 2;
    float cy = 720  / 2;

    // Level_Draw(game.level, 1280 / 2, 720 / 2);
    // HQC_Log("%p", Store_GetTextureByID(0));

    irect_t rect = {0, 0, 64, 64};
    HQC_Artist_DrawTexture(Store_GetTextureByID(TEX_FROG), cx, cy);
}