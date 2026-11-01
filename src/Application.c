#include "Application.h"

#include "global/Consts.h"
#include "global/Engine.h"
#include "global/Enums.h"


#include "gameplay/Game.h"
#include "menu/MenuMgr.h"
#include <stdio.h>

#define MAX_FPS 60
#define FRAME_DELAY 1000 / MAX_FPS

Engine engine;

typedef struct {
  Game game;
  MenuMgr menuMgr;

  int curLvl;
  int curDifficulty;

  int inMenu;
  int mouseClicked;

  uint32_t frameStart;
  int frameTime;

  bool isRunning;
} App;

static int ApplicationZuma_LoadResources(LevelMgr *levelMgr, Engine *engine) {
  if (!Engine_LoadSettings(engine))
    return 9;

  if (!Engine_TexturesLoad(engine, filesTextures, TEXTURES_COUNT)) {
    return 3;
  }
  if (!Engine_FontsLoad(engine, filesFonts, FONTS_COUNT))
    return 4;
  if (!Engine_SoundsLoad(engine, filesSounds, SOUNDS_COUNT))
    return 5;
  if (!Engine_SoundsSfxLoad(engine, filesSoundsSfx, SOUNDS_SFX_COUNT))
    return 6;
  if (!Engine_MusicLoad(engine, fileMusic))
    return 7;

  if (!LevelMgr_LoadLevels(levelMgr, "levels.xml"))
    return 8;
}

static int ApplicationZuma_ShowStartupImage(Engine *engine) {
  char *basePath = SDL_GetBasePath();

  char resPath[100];
  sprintf(resPath, "%simages/disclaimer.jpg", basePath);

  SDL_free(basePath);

  SDL_Texture *texDisc = Engine_TextureLoad(engine, resPath);
  if (!texDisc)
    return 10;

  SDL_RenderCopy(engine->render, texDisc, NULL, NULL);
  SDL_RenderPresent(engine->render);
  SDL_Delay(2000);
}

static void ApplicationZuma_HandleEvents(App *app) {
  app->mouseClicked = 0;

  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
    case SDL_QUIT:
      exit(0);
      break;
    case SDL_MOUSEBUTTONDOWN:

      switch (e.button.button) {
      case SDL_BUTTON_LEFT:
        app->mouseClicked = 1;
        break;
      case SDL_BUTTON_RIGHT:
        app->mouseClicked = 2;
        break;
      }
      break;
    }
  }
}

static void ApplicationZuma_Update(LevelMgr *levelMgr, Engine *engine, App *app) {
  SDL_RenderClear(engine->render);

  if (app->inMenu) {
    MenuMgr_Update(engine, &app->menuMgr);

    if (app->menuMgr.roomID == MR_GAME && app->inMenu) {
      app->inMenu = 0;
      Game_Init(levelMgr, engine, &app->game, app->curLvl, app->curDifficulty);
    }

    MenuMgr_Draw(levelMgr, engine, &app->menuMgr);
  } else {
    Game_Update(levelMgr, engine, &app->game, &app->inMenu, app->mouseClicked);
    if (app->inMenu) {
      MenuMgr_Set(engine, &app->menuMgr, MR_GAUNTLET);
      return;
    }

    Game_Draw(levelMgr, engine, &app->game);
  }

  SDL_RenderPresent(engine->render);
}

int ApplicationZuma_Start(void) {
  Engine engine;
  LevelMgr levelMgr;
  if (!Engine_Init(&engine))
    return 1;

  if (!Engine_CreateWindow(&engine, "Zuma HD. By GalaxyShad and s4lat", WINDOW_WIDTH, WINDOW_HEIGHT))
    return 2;

  ApplicationZuma_ShowStartupImage(&engine);
  ApplicationZuma_LoadResources(&levelMgr, &engine);

  srand(time(NULL));

  App app;
  app.curLvl = 0;
  app.curDifficulty = 0;
  app.inMenu = 1;
  app.isRunning = true;

  MenuMgr_Init(&app.menuMgr, &app.curLvl, &app.curDifficulty);
  MenuMgr_Set(&engine, &app.menuMgr, MR_MAIN);

  while (app.isRunning) {
    ApplicationZuma_HandleEvents(&app);

    app.frameStart = SDL_GetTicks();

    ApplicationZuma_Update(&levelMgr, &engine, &app);

    app.frameTime = SDL_GetTicks() - app.frameStart;

    if (app.frameTime < FRAME_DELAY)
      SDL_Delay(FRAME_DELAY - app.frameTime);
  }

  LevelMgr_Free(&levelMgr);
  Engine_Destroy(&engine);

  return 0;
}