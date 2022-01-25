#ifndef CONSTS_H
#define CONSTS_H

#define SOUNDS_SFX_COUNT 6
#define TEXTURES_COUNT 22 
#define SOUNDS_COUNT 38
#define FONTS_COUNT 9

const char *filesTextures[TEXTURES_COUNT] = {
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

const char *filesSounds[SOUNDS_COUNT] = {
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

const char *filesSoundsSfx[SOUNDS_SFX_COUNT] = {
    "chain1.ogg",
    "chime1.ogg",
    "choral1.ogg",
    "endoflevelpop1.ogg",
    "gapbonus1.ogg",
    "lighttrail2.ogg",
};

const char *filesFonts[FONTS_COUNT] = {
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

const char fileMusic[9] = "zuma.mo3";

#endif