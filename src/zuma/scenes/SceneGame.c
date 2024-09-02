#include "Index.h"

#include "../Level.h"
#include "../Frog.h"
#include "../BallChain.h"
#include "../FloatingText.h"
#include "../Statistics.h"

struct {
    HFrog  frog;
    HLevel level;
    HBallChain chain;
    HBulletList bulletList;
    HBallChainGenerator generator;
} game;

static void GoBack_() {
    Scene_Change(SC_TEST);
}

static void Game_Start_() {
    LevelSettings levelSettings;
    levelSettings.id = "some settings";

    LevelGraphics* levelGx       = HQC_Memory_Allocate(sizeof(*levelGx));
    levelGx->dispName            = "Coding is Hard :(";
    levelGx->coinsPosList        = HQC_Container_CreateVector(sizeof(v2f_t));
    levelGx->frogPos.x           = (450.f + 104) * 1.5f;
    levelGx->frogPos.y           = 240.f * 1.5f;
    levelGx->id                  = "longrange";
    levelGx->textureFile         = "levels/longrange/longrange.jpg";
    levelGx->textureTopLayerFile = NULL;
    levelGx->curveAFile          = "levels/longrange/longrange.dat";
    levelGx->curveBFile          = NULL;                                        

    game.level = Level_Load(&levelSettings, levelGx);

    game.bulletList = BulletList_Create();
    game.frog = Frog_Create(levelGx->frogPos.x, levelGx->frogPos.y, game.bulletList);
    game.chain = BallChain_Create(game.level, game.bulletList);
    game.generator = BallChainGenerator_Create(game.chain);

    //for (int i = 0; i < 20; i++)
      //  BallChain_AddToStart(game.chain, HQC_RandomRange(0, 1));

    HQC_Animation_SetSpeed(Store_GetAnimationByID(ANIM_SKULL), 0);

   // HQC_DJ_SetSoundPith(4);
    HQC_DJ_PlaySound(Store_GetSoundByID(SND_CHANT1));

    FloatingTextFactory_Init();

    Statistics_Init();
}


static void Game_Update__() {

    FloatingTextFactory_Update();
    BallChain_Update(game.chain);
    BallChainGenerator_Update(game.generator);

    Frog_Update(game.frog);
    BulletList_Update(game.bulletList);
}


static void Game_Draw__() {
    float cx = 1280.f / 2;
    float cy = 720.f  / 2;

    Level_Draw(game.level, cx, cy);

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

    BallChain_Draw(game.chain);
    BulletList_Draw(game.bulletList);

    Frog_DrawTop(game.frog);

    FloatingTextFactory_Draw();
}

static void Game_Free_() {

}

HScene Scene_Register_Game() {
    return Scene_New("game", Game_Start_, Game_Update__, Game_Draw__, Game_Free_);
}