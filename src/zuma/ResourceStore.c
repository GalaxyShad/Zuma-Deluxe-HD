#include "ResourceStore.h"

#define private static
#define public


static const char* _TEXTURE_FILES[] = {
    "frog.png",
    "gameobjects.png",
    "gameinterface.png",
    "menu.png",

    "thumbnails\\thumb (1).jpg",
    "thumbnails\\thumb (2).jpg",
    "thumbnails\\thumb (3).jpg",
    "thumbnails\\thumb (4).jpg",
    "thumbnails\\thumb (5).jpg",
    "thumbnails\\thumb (6).jpg",
    "thumbnails\\thumb (7).jpg",
    "thumbnails\\thumb (8).jpg",
    "thumbnails\\thumb (9).jpg",
    "thumbnails\\thumb (10).jpg",
    "thumbnails\\thumb (11).jpg",
    "thumbnails\\thumb (12).jpg",
    "thumbnails\\thumb (13).jpg",
    "thumbnails\\thumb (14).jpg",
    "thumbnails\\thumb (15).jpg",
    "thumbnails\\thumb (16).jpg",
    "thumbnails\\thumb (17).jpg",
    "thumbnails\\thumb (18).jpg",
};

static const char* _SOUND_FILES[] = {
    "accuracy3.ogg",
    "ballclick1.ogg",
    "ballclick2.ogg",
    "ballsdestroyed1.ogg",
    "ballsdestroyed2.ogg",
    "ballsdestroyed3.ogg",
    "ballsdestroyed4.ogg",
    "ballsdestroyed5.ogg",
    "bombexplode.ogg",
    "button1.ogg",
    "button2.ogg",
    "chain1.ogg",
    "chant1.ogg",
    "chant14.ogg",
    "chant2.ogg",
    "chant3.ogg",
    "chant4.ogg",
    "chant5.ogg",
    "chant6.ogg",
    "chant8.ogg",
    "chime1.ogg",
    "choral1.ogg",
    "coingrab.ogg",
    "earthquake.ogg",
    "endoflevelpop1.ogg",
    "extralife.ogg",
    "fireball1.ogg",
    "frogland2.ogg",
    "gapbonus1.ogg",
    "gemvanishes.ogg",
    "jewelappear.ogg",
    "lighttrail2.ogg",
    "pop.ogg",
    "reverse1.ogg",
    "rolling.ogg",
    "slowdown1.ogg",
    "ufo1.ogg",
    "warning1.ogg",
};

static const char* _MUSIC_FILE = "zuma.mo3";

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

private HQC_VECTOR(HQC_Texture)      _textureList    = NULL;

private HQC_VECTOR(HQC_Sprite)       _spriteList     = NULL;
private HQC_VECTOR(HQC_Animation)    _animationList  = NULL;

private HQC_VECTOR(HQC_Sound)        _soundList      = NULL;
private HQC_VECTOR(HQC_Font)         _fontList       = NULL;
private HQC_Music                    _music          = NULL;


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

private void _LoadTextures() {
    size_t count = sizeof(_TEXTURE_FILES) / sizeof(_TEXTURE_FILES[0]);

    _textureList = HQC_Container_CreateVector(sizeof(HQC_Texture));

    for (int i = 0; i < count; i++) {
        const char* path = HQC_StringConcat("images/", _TEXTURE_FILES[i]);
        HQC_Texture texture = HQC_Artist_LoadTexture(path);
        HQC_Container_VectorAdd(_textureList, &texture);
    }

    HQC_Log("[ResourceStore] Common textures count: %d", HQC_Container_VectorCount(_textureList));
}


private void _MakeSprites() {
    _spriteList = HQC_Container_CreateVector(sizeof(HQC_Sprite));

    HQC_Texture texFrog = Store_GetTextureByID(TEX_FROG);

    HQC_Sprite sprFrog = HQC_Sprite_Create(texFrog, 
        0,      0,      162,    162);
    HQC_Sprite sprFrogPlate = HQC_Sprite_Create(texFrog, 
        162,    162,    162,    162);
    HQC_Sprite sprFrogTongue = HQC_Sprite_Create(texFrog, 
        162,    0,      162,    162);

    HQC_Container_VectorAdd(_spriteList, &sprFrog);
    HQC_Container_VectorAdd(_spriteList, &sprFrogPlate);
    HQC_Container_VectorAdd(_spriteList, &sprFrogTongue);

    //////////////////////////////////////////////////////////////

    HQC_Texture texGameHud = Store_GetTextureByID(TEX_GAME_HUD);

    HQC_Sprite gameHudBorder = HQC_Sprite_Create(texGameHud,  
        0, 51, 1280, 730);
    HQC_Sprite gameHudLive = HQC_Sprite_Create(texGameHud,  
        40, 18, 40, 36);
    HQC_Sprite gameHudProgressBarGreen = HQC_Sprite_Create(texGameHud,  
        773, 0, 94, 27);
    HQC_Sprite gameHudProgressBarYellow = HQC_Sprite_Create(texGameHud,  
        773, 28, 94, 27);

    HQC_Sprite gameHudBtnMenu = HQC_Sprite_Create(texGameHud,  
        925, 15, 118, 37);
    HQC_Sprite gameHudBtnMenuHover = HQC_Sprite_Create(texGameHud,  
        1043, 15, 118, 37);
    HQC_Sprite gameHudBtnMenuPressed = HQC_Sprite_Create(texGameHud,  
        1162, 15, 118, 37);

    HQC_Container_VectorAdd(_spriteList, &gameHudBorder);
    HQC_Container_VectorAdd(_spriteList, &gameHudLive);
    HQC_Container_VectorAdd(_spriteList, &gameHudProgressBarGreen);
    HQC_Container_VectorAdd(_spriteList, &gameHudProgressBarYellow);
    HQC_Container_VectorAdd(_spriteList, &gameHudBtnMenu);
    HQC_Container_VectorAdd(_spriteList, &gameHudBtnMenuHover);
    HQC_Container_VectorAdd(_spriteList, &gameHudBtnMenuPressed);

    //////////////////////////////////////////////////////////////

    HQC_Texture texMenu = Store_GetTextureByID(TEX_MENU);

    HQC_Sprite menuDialogBoxRectLT = HQC_Sprite_Create(texMenu,  
        121*0, 121*0, 121, 121);
    HQC_Sprite menuDialogBoxRectCT = HQC_Sprite_Create(texMenu,  
        121*1, 121*0, 121, 121);
    HQC_Sprite menuDialogBoxRectRT = HQC_Sprite_Create(texMenu,  
        121*2, 121*0, 121, 121);
    HQC_Sprite menuDialogBoxRectLC = HQC_Sprite_Create(texMenu,  
        121*0, 121*1, 121, 121);
    HQC_Sprite menuDialogBoxRectCC = HQC_Sprite_Create(texMenu,  
        121*1, 121*1, 121, 121);
    HQC_Sprite menuDialogBoxRectRC = HQC_Sprite_Create(texMenu,  
        121*2, 121*1, 121, 121);
    HQC_Sprite menuDialogBoxRectLB = HQC_Sprite_Create(texMenu,  
        121*0, 121*2, 121, 121);
    HQC_Sprite menuDialogBoxRectCB = HQC_Sprite_Create(texMenu,  
        121*1, 121*2, 121, 121);
    HQC_Sprite menuDialogBoxRectRB = HQC_Sprite_Create(texMenu,  
        121*2, 121*2, 121, 121);

    HQC_Sprite menuSlider = HQC_Sprite_Create(texMenu,  
        0, 365, 307, 49);
    HQC_Sprite menuSliderThumb = HQC_Sprite_Create(texMenu,  
        0, 414,  61, 45);

    HQC_Sprite menuCheckboxChecked = HQC_Sprite_Create(texMenu,  
        0, 462, 75, 69);
    HQC_Sprite menuCheckbox = HQC_Sprite_Create(texMenu,  
        75, 462, 75, 69);

    HQC_Sprite menuButton = HQC_Sprite_Create(texMenu,  
        0, 532, 180, 72);
    HQC_Sprite menuButtonHover = HQC_Sprite_Create(texMenu,  
        180, 532, 180, 72);
    HQC_Sprite menuButtonPressed = HQC_Sprite_Create(texMenu,  
        180*2, 532, 180, 72);

    HQC_Sprite menuHead = HQC_Sprite_Create(texMenu,  
        363, 0, 234, 210);

    HQC_Sprite menuLoadBar = HQC_Sprite_Create(texMenu,  
        685, 9, 593, 50);

    HQC_Sprite menuScreenLoading = HQC_Sprite_Create(texMenu,  
        1282, 720*0, 1280, 720);
    HQC_Sprite menuScreenMain = HQC_Sprite_Create(texMenu,  
        1282, 720*1, 1280, 720);
    HQC_Sprite menuScreenGauntlet = HQC_Sprite_Create(texMenu,  
        1282, 720*2, 1280, 720);

    HQC_Sprite menuMainSun = HQC_Sprite_Create(texMenu,  
        2563, 315, 130, 138);
    HQC_Sprite menuMainSunLight = HQC_Sprite_Create(texMenu,  
        2563, 0, 315, 315);

    HQC_Sprite menuScreenMainSky = HQC_Sprite_Create(texMenu,  
        10, 720, 1280, 250);
    HQC_Sprite menuScreenGauntSky = HQC_Sprite_Create(texMenu,  
        0, 1442, 1280, 250);

    HQC_Sprite menuMainBtnOptions = HQC_Sprite_Create(texMenu,  
        2563+299*0, 647, 299, 129);
    HQC_Sprite menuMainBtnOptionsHover = HQC_Sprite_Create(texMenu,  
        2563+299*1, 647, 299, 129);
    HQC_Sprite menuMainBtnOptionsPressed = HQC_Sprite_Create(texMenu,  
        2563+299*2, 647, 299, 129);

    HQC_Sprite menuMainBtnGauntlet = HQC_Sprite_Create(texMenu,  
        2648+270*0, 776, 270, 125);
    HQC_Sprite menuMainBtnGauntletHover = HQC_Sprite_Create(texMenu,  
        2648+270*1, 776, 270, 125);
    HQC_Sprite menuMainBtnGauntletPressed = HQC_Sprite_Create(texMenu,  
        2648+270*2, 776, 270, 125);

    HQC_Sprite menuMainBtnAdventure = HQC_Sprite_Create(texMenu,  
        2724+245*0, 901, 245, 138);
    HQC_Sprite menuMainBtnAdventureHover = HQC_Sprite_Create(texMenu,  
        2724+245*1, 901, 245, 138);
    HQC_Sprite menuMainBtnAdventurePressed = HQC_Sprite_Create(texMenu,  
        2724+245*2, 901, 245, 138);

    HQC_Sprite menuMainBtnMoreGames = HQC_Sprite_Create(texMenu,  
        2924+177*0, 1038, 177, 190);
    HQC_Sprite menuMainBtnMoreGamesHover = HQC_Sprite_Create(texMenu,  
        2924+177*1, 1038, 177, 190);
    HQC_Sprite menuMainBtnMoreGamesPressed = HQC_Sprite_Create(texMenu,  
        2924+177*2, 1038, 177, 190);

    HQC_Sprite menuMainBtnQuit = HQC_Sprite_Create(texMenu,  
        2917+180*0, 1226, 180, 211);
    HQC_Sprite menuMainBtnQuitHover = HQC_Sprite_Create(texMenu,  
        2917+180*1, 1226, 180, 211);
    HQC_Sprite menuMainBtnQuitPressed = HQC_Sprite_Create(texMenu,  
        2917+180*2, 1226, 180, 211);

    HQC_Sprite menuGauntBtnMainMenu = HQC_Sprite_Create(texMenu,  
        1280+211*0, 2160, 211, 84);
    HQC_Sprite menuGauntBtnMainMenuHover = HQC_Sprite_Create(texMenu,  
        1280+211*1, 2160, 211, 84);
    HQC_Sprite menuGauntBtnMainMenuPressed = HQC_Sprite_Create(texMenu,  
        1280+211*2, 2160, 211, 84);

    HQC_Sprite menuGauntBtnBack = HQC_Sprite_Create(texMenu,  
        1924+139*0, 2260, 139, 48);
    HQC_Sprite menuGauntBtnBackHover = HQC_Sprite_Create(texMenu,  
        1924+139*1, 2260, 139, 48);
    HQC_Sprite menuGauntBtnBackPressed = HQC_Sprite_Create(texMenu,  
        1924+139*2, 2260, 139, 48);

    HQC_Sprite menuGauntBtnPlay = HQC_Sprite_Create(texMenu,  
        1922+157*0, 2210, 157, 49);
    HQC_Sprite menuGauntBtnPlayHover = HQC_Sprite_Create(texMenu,  
        1922+157*1, 2210, 157, 49);
    HQC_Sprite menuGauntBtnPlayPressed = HQC_Sprite_Create(texMenu,  
        1922+157*2, 2210, 157, 49);

    HQC_Sprite menuGauntBtnNext = HQC_Sprite_Create(texMenu,  
        1922+141*0, 2161, 141, 48);
    HQC_Sprite menuGauntBtnNextHover = HQC_Sprite_Create(texMenu,  
        1922+141*1, 2161, 141, 48);
    HQC_Sprite menuGauntBtnNextPressed = HQC_Sprite_Create(texMenu,  
        1922+141*2, 2161, 141, 48);

    HQC_Sprite menuGauntBtnRabbitDisabled = HQC_Sprite_Create(texMenu,  
        1280+328*0, 2336, 328, 63);
    HQC_Sprite menuGauntBtnRabbit = HQC_Sprite_Create(texMenu,  
        1280+328*1, 2336, 328, 63);
    HQC_Sprite menuGauntBtnRabbitHover = HQC_Sprite_Create(texMenu,  
        1280+328*2, 2336, 328, 63);
    HQC_Sprite menuGauntBtnRabbitPressed = HQC_Sprite_Create(texMenu,  
        1280+328*3, 2336, 328, 63);

    HQC_Sprite menuGauntBtnEagleDisabled = HQC_Sprite_Create(texMenu,  
        1280+258*0, 2399, 258, 49);
    HQC_Sprite menuGauntBtnEagle = HQC_Sprite_Create(texMenu,  
        1280+258*1, 2399, 258, 49);
    HQC_Sprite menuGauntBtnEagleHover = HQC_Sprite_Create(texMenu,  
        1280+258*2, 2399, 258, 49);
    HQC_Sprite menuGauntBtnEaglePressed = HQC_Sprite_Create(texMenu,  
        1280+258*3, 2399, 258, 49);

    HQC_Sprite menuGauntBtnJaguarDisabled = HQC_Sprite_Create(texMenu,  
        1280+205*0, 2448, 205, 43);
    HQC_Sprite menuGauntBtnJaguar = HQC_Sprite_Create(texMenu,  
        1280+205*1, 2448, 205, 43);
    HQC_Sprite menuGauntBtnJaguarHover = HQC_Sprite_Create(texMenu,  
        1280+205*2, 2448, 205, 43);
    HQC_Sprite menuGauntBtnJaguarPressed = HQC_Sprite_Create(texMenu,  
        1280+205*3, 2448, 205, 43);

    HQC_Sprite menuGauntBtnSunGodDisabled = HQC_Sprite_Create(texMenu,  
        1280+159*0, 2491, 159, 54);
    HQC_Sprite menuGauntBtnSunGod = HQC_Sprite_Create(texMenu,  
        1280+159*1, 2491, 159, 54);
    HQC_Sprite menuGauntBtnSunGodHover = HQC_Sprite_Create(texMenu,  
        1280+159*2, 2491, 159, 54);
    HQC_Sprite menuGauntBtnSunGodPressed = HQC_Sprite_Create(texMenu,  
        1280+159*3, 2491, 159, 54);

    HQC_Container_VectorAdd(_spriteList, &menuDialogBoxRectLT);
    HQC_Container_VectorAdd(_spriteList, &menuDialogBoxRectCT);
    HQC_Container_VectorAdd(_spriteList, &menuDialogBoxRectRT);
    HQC_Container_VectorAdd(_spriteList, &menuDialogBoxRectLC);
    HQC_Container_VectorAdd(_spriteList, &menuDialogBoxRectCC);
    HQC_Container_VectorAdd(_spriteList, &menuDialogBoxRectRC);
    HQC_Container_VectorAdd(_spriteList, &menuDialogBoxRectLB);
    HQC_Container_VectorAdd(_spriteList, &menuDialogBoxRectCB);
    HQC_Container_VectorAdd(_spriteList, &menuDialogBoxRectRB);
    HQC_Container_VectorAdd(_spriteList, &menuSlider);
    HQC_Container_VectorAdd(_spriteList, &menuSliderThumb);
    HQC_Container_VectorAdd(_spriteList, &menuCheckboxChecked);
    HQC_Container_VectorAdd(_spriteList, &menuCheckbox);
    HQC_Container_VectorAdd(_spriteList, &menuButton);
    HQC_Container_VectorAdd(_spriteList, &menuButtonHover);
    HQC_Container_VectorAdd(_spriteList, &menuButtonPressed);
    HQC_Container_VectorAdd(_spriteList, &menuHead);
    HQC_Container_VectorAdd(_spriteList, &menuLoadBar);
    HQC_Container_VectorAdd(_spriteList, &menuScreenLoading);
    HQC_Container_VectorAdd(_spriteList, &menuScreenMain);
    HQC_Container_VectorAdd(_spriteList, &menuScreenGauntlet);
    HQC_Container_VectorAdd(_spriteList, &menuMainSun);
    HQC_Container_VectorAdd(_spriteList, &menuMainSunLight);
    HQC_Container_VectorAdd(_spriteList, &menuScreenMainSky);
    HQC_Container_VectorAdd(_spriteList, &menuScreenGauntSky);
    HQC_Container_VectorAdd(_spriteList, &menuMainBtnOptions);
    HQC_Container_VectorAdd(_spriteList, &menuMainBtnOptionsHover);
    HQC_Container_VectorAdd(_spriteList, &menuMainBtnOptionsPressed);
    HQC_Container_VectorAdd(_spriteList, &menuMainBtnGauntlet);
    HQC_Container_VectorAdd(_spriteList, &menuMainBtnGauntletHover);
    HQC_Container_VectorAdd(_spriteList, &menuMainBtnGauntletPressed);
    HQC_Container_VectorAdd(_spriteList, &menuMainBtnAdventure);
    HQC_Container_VectorAdd(_spriteList, &menuMainBtnAdventureHover);
    HQC_Container_VectorAdd(_spriteList, &menuMainBtnAdventurePressed);
    HQC_Container_VectorAdd(_spriteList, &menuMainBtnMoreGames);
    HQC_Container_VectorAdd(_spriteList, &menuMainBtnMoreGamesHover);
    HQC_Container_VectorAdd(_spriteList, &menuMainBtnMoreGamesPressed);
    HQC_Container_VectorAdd(_spriteList, &menuMainBtnQuit);
    HQC_Container_VectorAdd(_spriteList, &menuMainBtnQuitHover);
    HQC_Container_VectorAdd(_spriteList, &menuMainBtnQuitPressed);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnMainMenu);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnMainMenuHover);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnMainMenuPressed);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnBack);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnBackHover);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnBackPressed);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnPlay);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnPlayHover);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnPlayPressed);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnNext);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnNextHover);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnNextPressed);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnRabbitDisabled);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnRabbit);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnRabbitHover);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnRabbitPressed);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnEagleDisabled);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnEagle);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnEagleHover);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnEaglePressed);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnJaguarDisabled);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnJaguar);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnJaguarHover);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnJaguarPressed);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnSunGodDisabled);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnSunGod);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnSunGodHover);
    HQC_Container_VectorAdd(_spriteList, &menuGauntBtnSunGodPressed);

    //////////////////////////////////////////////////////////////

    HQC_Log("[ResourceStore] Common sprite mappings count: %d", HQC_Container_VectorCount(_spriteList));
}


private HQC_Animation _MakeAnimationFromStrip(HQC_Texture tex, int framesCount, irect_t frameRect) {
    HQC_Animation anim = HQC_Animation_Create();

    for (int i = 0; i < framesCount; i++) {
        HQC_Sprite frame = HQC_Sprite_Create(
            tex, frameRect.x, frameRect.y, frameRect.width, frameRect.height);

        HQC_Animation_AddFrame(anim, frame);

        frameRect.y += frameRect.height;
    }

    return anim;
}


private void _MakeAnimations() {
    _animationList = HQC_Container_CreateVector(sizeof(HQC_Animation));
    
    HQC_Texture texObjects = Store_GetTextureByID(TEX_GAME_OBJECTS);

    ////////////////////////////////////////////////////////////////////////////
    //    Balls
    ////////////////////////////////////////////////////////////////////////////

    irect_t rectBall = { 0, 0, 48, 48 };
    for (int c = 0; c < 6; c++) {
        HQC_Animation animBall = 
            _MakeAnimationFromStrip(texObjects, (c == 0) ? 47 : 50, rectBall);

        HQC_Container_VectorAdd(_animationList, &animBall);

        rectBall.x += rectBall.width;
        rectBall.y = 0;
    }

    ////////////////////////////////////////////////////////////////////////////
    //    Coin
    ////////////////////////////////////////////////////////////////////////////
    
    irect_t rectCoin = { 340, 0, 54, 54 };
    HQC_Animation animCoin = 
        _MakeAnimationFromStrip(texObjects, 30, rectCoin);
    HQC_Container_VectorAdd(_animationList, &animCoin);

    ////////////////////////////////////////////////////////////////////////////
    //    Ball Destroy Effect
    ////////////////////////////////////////////////////////////////////////////
    
    irect_t rectBallDestroy = { 395, 0, 105, 120 };
    HQC_Animation animBallDestroy = 
        _MakeAnimationFromStrip(texObjects, 13, rectBallDestroy);
    HQC_Container_VectorAdd(_animationList, &animBallDestroy);

    ////////////////////////////////////////////////////////////////////////////
    //    Explosion
    ////////////////////////////////////////////////////////////////////////////
    
    irect_t rectExplosion = { 528, 0, 100, 130 };
    HQC_Animation animExplosion = 
        _MakeAnimationFromStrip(texObjects, 17, rectExplosion);
    HQC_Container_VectorAdd(_animationList, &animExplosion);

    ////////////////////////////////////////////////////////////////////////////
    //    Skull
    ////////////////////////////////////////////////////////////////////////////
    
    irect_t rectSkull = { 629, 132, 132, 132 };
    HQC_Animation animSkull = 
        _MakeAnimationFromStrip(texObjects, 12, rectSkull);
    HQC_Container_VectorAdd(_animationList, &animSkull);

    ////////////////////////////////////////////////////////////////////////////
    //    Sparkle
    ////////////////////////////////////////////////////////////////////////////
    
    irect_t rectSparkle = { 732, 2253, 30, 30 };
    HQC_Animation animSparkle = 
        _MakeAnimationFromStrip(texObjects, 14, rectSparkle);
    HQC_Container_VectorAdd(_animationList, &animSparkle);



    HQC_Log("[ResourceStore] Common animations count: %d", HQC_Container_VectorCount(_animationList));

}


private void _LoadSounds() {
    size_t count = sizeof(_SOUND_FILES) / sizeof(_SOUND_FILES[0]);

    _soundList = HQC_Container_CreateVector(sizeof(HQC_Sound));
    for (int i = 0; i < count; i++) {
        const char* path = HQC_StringConcat("sounds/", _SOUND_FILES[i]);
        HQC_Sound sound = HQC_DJ_LoadSound(path);
        HQC_Container_VectorAdd(_soundList, &sound);
    }

    HQC_Log("[ResourceStore] Common sounds count: %d", HQC_Container_VectorCount(_soundList));
}

 
private void _LoadFonts() {
    _fontList = HQC_Container_CreateVector(sizeof(HQC_Font));
    
    HQC_Font cancun10 = HQC_Font_LoadTrueType("fonts/ttf/Cancun Regular.ttf", 20);
    HQC_Font cancun12 = HQC_Font_LoadTrueType("fonts/ttf/Cancun Regular.ttf", 24);
    HQC_Font cancun13 = HQC_Font_LoadTrueType("fonts/ttf/Cancun Regular.ttf", 26);
    HQC_Font cancun8  = HQC_Font_LoadTrueType("fonts/ttf/Cancun Regular.ttf", 16);
    HQC_Font nativea  = HQC_Font_LoadTrueType("fonts/ttf/native.ttf", 96);

    HQC_Container_VectorAdd(_fontList, &cancun10);
    HQC_Container_VectorAdd(_fontList, &cancun12);
    HQC_Container_VectorAdd(_fontList, &cancun13);
    HQC_Container_VectorAdd(_fontList, &cancun8);
    HQC_Container_VectorAdd(_fontList, &nativea);

    HQC_Log("[ResourceStore] Common fonts count: %d", HQC_Container_VectorCount(_fontList));
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

public void Store_LoadAll() {
    _LoadTextures();
    _MakeSprites();
    _MakeAnimations();
    _LoadSounds();
    _LoadFonts();
}

 
public HQC_Texture Store_GetTextureByID(int id) {
    return *((HQC_Texture*)HQC_Container_VectorGet(_textureList, id));
}


public HQC_Sprite  Store_GetSpriteByID(int id) {
    return *((HQC_Sprite*)HQC_Container_VectorGet(_spriteList, id));
}


public HQC_Animation  Store_GetAnimationByID(int id) {
    return *((HQC_Animation*)HQC_Container_VectorGet(_animationList, id));
}


public HQC_Font Store_GetFontByID(int id) {
    return *((HQC_Font*)HQC_Container_VectorGet(_fontList, id));
}

public HQC_Sound Store_GetSoundByID(int id) {
    return *((HQC_Sound*)HQC_Container_VectorGet(_soundList, id));
}


public int Store_CountSounds() {
    return HQC_Container_VectorCount(_soundList);
}


public int Store_CountSprites() {
    return HQC_Container_VectorCount(_spriteList);
}