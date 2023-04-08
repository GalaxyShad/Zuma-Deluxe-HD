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

static const char* _FONT_FILES[] = {
    "arial12bold",
    "cancun10",
    "cancun12",
    "cancun13",
    "cancun8",
    //"cancunfat8",
    "cancunfloat14",
    //"cancunoutline10",
    "nativealien48",
    "nativealienextended16",
    "nativealienextended18",
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

    HQC_Log("Size %d", sizeof(HQC_Texture));
    
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

    HQC_Sprite sprFrog = HQC_Sprite_Create(
        texFrog, 0, 0, 162, 162);
    HQC_Sprite sprFrogPlate = HQC_Sprite_Create(
        texFrog, 162, 162, 162, 162);
    HQC_Sprite sprFrogTongue = HQC_Sprite_Create(
        texFrog, 162, 0, 162, 162);

    HQC_Container_VectorAdd(_spriteList, &sprFrog);
    HQC_Container_VectorAdd(_spriteList, &sprFrogPlate);
    HQC_Container_VectorAdd(_spriteList, &sprFrogTongue);
}


private void _MakeAnimations() {
    _animationList = HQC_Container_CreateVector(sizeof(HQC_Animation));
    
    HQC_Texture texObjects = Store_GetTextureByID(TEX_GAME_OBJECTS);

    irect_t rectBall = { 0, 0, 48, 48 };
    for (int c = 0; c < 7; c++) {
        HQC_Animation animBall = HQC_Animation_Create();

        for (int i = 0; i < 50; i++) {
            HQC_Sprite sprFrameBall = HQC_Sprite_Create(
                texObjects,
                rectBall.x, rectBall.y, rectBall.width, rectBall.height
            );

            HQC_Animation_AddFrame(animBall, sprFrameBall);

            rectBall.y += rectBall.height;
        }

        HQC_Container_VectorAdd(_animationList, &animBall);

        rectBall.x += rectBall.width;
        rectBall.y = 0;
    }


}


private void _LoadSounds() {
    // size_t count = sizeof(_SOUND_FILES) / sizeof(_SOUND_FILES[0]);

    // _soundList = HQC_Container_CreateVector(sizeof(HQC_Sound));
    // for (int i = 0; i < count; i++)
    //     HQC_Container_VectorAdd(_soundList, HQC_DJ_LoadSound(_SOUND_FILES[i]));

    // HQC_Log("[ResourceStore] Common sounds count: %d", HQC_Container_VectorCount(_soundList));
}


private void _LoadFonts() {
    // size_t count = sizeof(_FONT_FILES) / sizeof(_FONT_FILES[0]);

    // _fontList = HQC_Container_CreateVector(sizeof(HQC_Font));
    // for (int i = 0; i < count; i++)
    //     HQC_Container_VectorAdd(_fontList, HQC_Artist_LoadFont(_FONT_FILES[i]));

    // HQC_Log("[ResourceStore] Common fonts count: %d", HQC_Container_VectorCount(_fontList));
}




//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

public void Store_LoadAll() {
    _LoadTextures();
    _MakeSprites();
    _MakeAnimations();
    // _LoadSounds();
    // _LoadFonts();
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