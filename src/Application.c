#include "Application.h"

#include "global/Consts.h"
#include "global/Enums.h"
#include "global/Engine.h"

#include "gameplay/Game.h"
#include "menu/MenuMgr.h"

#define MAX_FPS 60
#define FRAME_DELAY 1000 / MAX_FPS

static struct {
    Game game;
    MenuMgr menuMgr;

    int curLvl;
    int curDifficulty;

    int inMenu;
    int mouseClicked;

    uint32_t frameStart;
    int frameTime;

    bool isRunning;
} app;


static int ApplicationZuma_LoadResources(void) {
    if (!Engine_LoadSettings())
        return 9;

    if (!Engine_TexturesLoad(filesTextures, TEXTURES_COUNT))
        return 3;
    if (!Engine_FontsLoad(filesFonts, FONTS_COUNT))
        return 4;
    if (!Engine_SoundsLoad(filesSounds, SOUNDS_COUNT))
        return 5;
    if (!Engine_SoundsSfxLoad(filesSoundsSfx, SOUNDS_SFX_COUNT))
        return 6;
    if (!Engine_MusicLoad(fileMusic))
        return 7;

    if (!LevelMgr_LoadLevels("levels.xml"))
        return 8;
}


static int ApplicationZuma_ShowStartupImage(void) {
    SDL_Texture* texDisc = Engine_TextureLoad("images\\disclaimer.jpg");
    if (!texDisc)
        return 10;

    SDL_RenderCopy(engine.render, texDisc, NULL, NULL);
    SDL_RenderPresent(engine.render);
    SDL_Delay(2000);
}


static void ApplicationZuma_HandleEvents(void) {
    app.mouseClicked = 0;

    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
        switch (e.type) {
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_MOUSEBUTTONDOWN:

                switch (e.button.button) {
                    case SDL_BUTTON_LEFT: 
                        app.mouseClicked = 1;
                        break;
                    case SDL_BUTTON_RIGHT: 
                        app.mouseClicked = 2;
                        break;

                }
            break;
        }
    }
}


static void ApplicationZuma_Update(void) {
    SDL_RenderClear(engine.render);
    if (app.inMenu) {
        MenuMgr_Update(&app.menuMgr);
        if (app.menuMgr.roomID == MR_GAME && app.inMenu) {
            app.inMenu = 0;
            Game_Init(&app.game, app.curLvl, app.curDifficulty);
        }

        MenuMgr_Draw(&app.menuMgr);
    } else {
        Game_Update(&app.game, &app.inMenu, app.mouseClicked);
        if (app.inMenu) {
            MenuMgr_Set(&app.menuMgr, MR_GAUNTLET);
            return;
        }

        Game_Draw(&app.game);
    }
    SDL_RenderPresent(engine.render);
}


int ApplicationZuma_Start(void) {
    if (!Engine_Init())
        return 1;

    if (!Engine_CreateWindow("Zuma HD. By GalaxyShad and s4lat", WINDOW_WIDTH, WINDOW_HEIGHT))
        return 2;

    ApplicationZuma_ShowStartupImage();
    ApplicationZuma_LoadResources();

    srand(time(NULL));

    app.curLvl = 0;
    app.curDifficulty = 0;
    app.inMenu = 1;
    app.isRunning = true;

    MenuMgr_Init(&app.menuMgr, &app.curLvl, &app.curDifficulty);
    MenuMgr_Set(&app.menuMgr, MR_MAIN);

    while (app.isRunning) {
        ApplicationZuma_HandleEvents();

        app.frameStart = SDL_GetTicks();

        ApplicationZuma_Update();

        app.frameTime = SDL_GetTicks() - app.frameStart;

        if (app.frameTime < FRAME_DELAY)
            SDL_Delay(FRAME_DELAY - app.frameTime);
    }

    LevelMgr_Free();
    Engine_Destroy();

    return 0;
}