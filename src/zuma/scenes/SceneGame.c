#include "Index.h"

#include "../Level.h"
#include "../Frog.h"
#include "../ResourceStore.h"
#include "../Menu.h"

struct {
    HFrog  frog;
    HLevel level;
    HButton btn;
} game;


static void _Game_Start() {
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

    game.btn = Button_Create(312, 312);

    HQC_DJ_SetSoundPith(2);
    HQC_DJ_PlaySound(Store_GetSoundByID(SND_CHANT1));
}


static void _Game_Update() {
    Frog_Update(game.frog);
    Button_Update(game.btn);
}


static void _Game_Draw() {
    float cx = 1280 / 2;
    float cy = 720  / 2;

    Level_Draw(game.level, 1280 / 2, 720 / 2);       

    Frog_Draw(game.frog);

    for (int i = 0; i < 11; i++) {
        HQC_Animation_Tick(Store_GetAnimationByID(i));
        HQC_Artist_DrawAnimation(Store_GetAnimationByID(i), 78 + 64*i, 78);
    }

    HQC_Artist_DrawSprite(Store_GetSpriteByID(SPR_GAME_HUD_BORDER), cx, cy);

    HQC_Artist_DrawTextShadow(Store_GetFontByID(FONT_CANCUN_12), 
        "Hello", cx, 16);
    
    HQC_Artist_SetColorHex(0xffb347);
    HQC_Artist_DrawTextShadow(Store_GetFontByID(FONT_NATIVE_ALIEN_48), 
        Level_GetDisplayName(game.level), cx, 600);

    HQC_Artist_SetColorHex(C_WHITE);

    HQC_Artist_DrawSprite(Store_GetSpriteByID(SPR_GAME_HUD_LIVE), 64, 24);

    Button_Draw(game.btn);
}

static void _Game_Free() {

}

HScene Scene_Register_Game() {
    return Scene_New("game", _Game_Start, _Game_Update, _Game_Draw, _Game_Free); 
}