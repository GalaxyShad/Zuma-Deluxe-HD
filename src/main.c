#include "Consts.h"
#include "Enums.h"
#include "Engine.h"
#include "Game.h"
#include "MenuMgr.h"

#define MAX_FPS 60
#define FRAME_DELAY 1000 / MAX_FPS

int main(int argc, char ** args) { 
    if (!Engine_Init())
        return 1;

    if (!Engine_CreateWindow("Zuma HD. By GalaxyShad and s4lat", WINDOW_WIDTH, WINDOW_HEIGHT))
        return 2;

    SDL_Texture* texDisc = Engine_TextureLoad("images\\disclaimer.jpg");
    if (!texDisc)
        return 10;

    SDL_RenderCopy(engine.render, texDisc, NULL, NULL);
    SDL_RenderPresent(engine.render);
    SDL_Delay(2000);

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

    srand(time(NULL));

    Game game;
    MenuMgr menuMgr;

    int curLvl = 0;
    int curDifficulty = 0;

    MenuMgr_Init(&menuMgr, &curLvl, &curDifficulty);
    MenuMgr_Set(&menuMgr, MR_MAIN);

    int inMenu = 1;
    int mouseClicked = 0;

    uint32_t frameStart;
    int frameTime;

    while(1) {
        mouseClicked = 0;
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT:
                    exit(0);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    switch (e.button.button) {
                        case SDL_BUTTON_LEFT: 
                            mouseClicked = 1;
                            break;
                        case SDL_BUTTON_RIGHT: 
                            mouseClicked = 2;
                            break;

                    }
                break;
            }
        }

        frameStart = SDL_GetTicks();

        SDL_RenderClear(engine.render);
        if (inMenu) {
            MenuMgr_Update(&menuMgr);
            if (menuMgr.roomID == MR_GAME && inMenu) {
                inMenu = 0;
                Game_Init(&game, curLvl, curDifficulty);
            }

            MenuMgr_Draw(&menuMgr);
        } else {
            Game_Update(&game, &inMenu, mouseClicked);
            if (inMenu) {
                MenuMgr_Set(&menuMgr, MR_GAUNTLET);
                continue;
            }

            Game_Draw(&game);
        }
        SDL_RenderPresent(engine.render);

        frameTime = SDL_GetTicks() - frameStart;

        if (frameTime < FRAME_DELAY)
            SDL_Delay(FRAME_DELAY - frameTime);
    }

    LevelMgr_Free();
    Engine_Destroy();
    return 0;
}