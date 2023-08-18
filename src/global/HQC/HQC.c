#include "../HQC.h"

#include <SDL.h>
#include <SDL_Image.h>
#include <bass.h>

#include <SDL_ttf.h>

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
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        _RaiseSDLError("Init fail");

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (!(IMG_Init(flags) & flags))
        HQC_RaiseErrorHeaderFormat(
            "SDL Image Error", "Initialization fail [%s]", IMG_GetError());


    if (TTF_Init() != 0)
        _RaiseSDLError("SDL TTF Init Fail");
}


static void _AudioInit() {
    if (BASS_Init(
        -1,
        MUSIC_FREQUENCY,
        BASS_DEVICE_STEREO,
        0,
        NULL) == 0
    ) {
        HQC_RaiseErrorHeaderFormat("BASS DLL", "Initialization fail [%d]", BASS_ErrorGetCode());
    }
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

float HQC_Lerp(float start, float end, float val) {
    return start * (1.0 - val) + end * val;
}

float HQC_FAtan2(float y, float x) {
    return atan2f(y, x);
}

float HQC_FSin(float rad) {
    return sinf(rad);
}


float HQC_FCos(float rad) {
    return cosf(rad);
}

float HQC_FSqrt(float x) {
    return sqrtf(x);
}

float HQC_FMin(float a, float b) {
    return (a < b) ? a : b;
}

float HQC_FMax(float a, float b) {
    return (a > b) ? a : b;
}

float HQC_PointDistance(float x1, float y1, float x2, float y2) {
    float h = (x2 - x1);
    float v = (y2 - y1);
    
    return HQC_FSqrt(h * h + v * v);
}