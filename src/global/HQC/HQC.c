#include "../HQC.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bass.h>

#include <SDL2/SDL_ttf.h>

#define MUSIC_FREQUENCY 44100

#define WINDOW_WIDTH 1280.0
#define WINDOW_HEIGHT 720.0


void HQC_Delay(uint32_t ms) {
    SDL_Delay(ms);
}


uint32_t HQC_GetTicks() {
    return SDL_GetTicks();
}

int HQC_RandomRange(int min, int max) {
    return min + rand() % (max+1);
}


static void _RaiseSDLError(const char* errmsg) {
    HQC_RaiseErrorHeaderFormat(
        "SDL Error",
        "%s [%s]",
        errmsg, SDL_GetError()
    );
}


static void _RaiseSDLImageError(const char* errmsg) {
    HQC_RaiseErrorHeaderFormat(
        "SDL Image Error",
        "%s [%s]",
        errmsg, IMG_GetError()
    );
}

static void _VideoInit() {
    SDL_Init(SDL_INIT_VIDEO)    ? _RaiseSDLError("Init fail") : NULL;

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (!(IMG_Init(flags) & flags))
        HQC_RaiseErrorHeaderFormat(
            "SDL Image Error", "Initialization fail [%s]", IMG_GetError());

    (TTF_Init() != 0) ?  _RaiseSDLError("SDL TTF Init Fail") : NULL;
}


static void _AudioInit() {
    BASS_Init(
        -1, 
        MUSIC_FREQUENCY, 
        BASS_DEVICE_STEREO, 
        0, 
        NULL
    ) ? NULL : HQC_RaiseErrorHeaderFormat("BASS DLL", "Initialization fail [%d]", BASS_ErrorGetCode());
}



void HQC_Init() {
    _VideoInit();
    _AudioInit();
}


void HQC_Cleanup() {
    IMG_Quit();
    SDL_Quit();
    BASS_Free();
}

float HQC_RadianToDegrees(float radian) {
    return radian * 57.2958f;
}

float HQC_DegreesToRadian(float degree) {
    return degree * 0.0174533f;
}

