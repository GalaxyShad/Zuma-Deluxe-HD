#include "Index.h"

#include "../Level.h"
#include "../Frog.h"
#include "../BallChain.h"
#include "../FloatingText.h"
#include "../Statistics.h"

#include "../ecs/World.h"

#include "../systems/SpriteDrawSystem.h"
#include "../entities/FrogEntity.h"

struct {
    HFrog  frog;
    HLevel level;
    HBallChain chain;
    HBulletList bulletList;
    HBallChainGenerator generator;

    World* world;
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

    game.world = World_Create();

    FrogEntity_AddToWorld(game.world, 32, 32);
    FrogEntity_AddToWorld(game.world, 128, 256);
    FrogEntity_AddToWorld(game.world, 100, 600);

    TestEntity_AddToWorld(game.world, 200, 400, 0);
    TestEntity_AddToWorld(game.world, 200, 400, 140);
    TestEntity_AddToWorld(game.world, 200, 400, -140);

    HudEntity_AddToWorld(game.world, game.level);

    World_AddSystem(game.world, SpriteDrawSystem());
    World_AddSystem(game.world, HudSystem());

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

    World_RunSystems(game.world);
}


static void Game_Draw__() {
    float cx = 1280.f / 2;
    float cy = 720.f  / 2;

    Level_Draw(game.level, cx, cy);

    Frog_Draw(game.frog);



    BallChain_Draw(game.chain);
    BulletList_Draw(game.bulletList);

    Frog_DrawTop(game.frog);

    FloatingTextFactory_Draw();

    World_DrawSystems(game.world);
}

static void Game_Free_() {
  World_Destroy(game.world);
}

HScene Scene_Register_Game() {
    return Scene_New("game", Game_Start_, Game_Update__, Game_Draw__, Game_Free_);
}