#include "../HQC.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bass.h>

#define MUSIC_FREQUENCY 44100

#define WINDOW_WIDTH 1280.0
#define WINDOW_HEIGHT 720.0

static struct {
    SDL_Window*     window;
    SDL_Renderer* 	render;
} graphics;


static void _RaiseSDLError(const char* errmsg) {
    HQC_RaiseErrorHeaderFormat(
        "SDL Error",
        "%s [%d]",
        errmsg, SDL_GetError()
    );
}


static void _RaiseSDLImageError(const char* errmsg) {
    HQC_RaiseErrorHeaderFormat(
        "SDL Image Error",
        "%s [%d]",
        errmsg, IMG_GetError()
    );
}

static void _VideoInit() {
    SDL_Init(SDL_INIT_VIDEO)    ? NULL : _RaiseSDLError("Init fail");

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (!(IMG_Init(flags) & flags))
        HQC_RaiseErrorHeaderFormat("SDL Image Error", "Initialization fail [%d]", IMG_GetError());
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


void HQC_CreateWindow(const char* caption, int width, int height) {
    graphics.window = SDL_CreateWindow(
        caption, 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        width, height, 
        SDL_WINDOW_OPENGL
    );

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    graphics.render = SDL_CreateRenderer(
        graphics.window, 
        -1, 
        SDL_RENDERER_ACCELERATED | 
        SDL_RENDERER_PRESENTVSYNC 
    );

    if (!graphics.render)
        _RaiseSDLError("Cannot create render");

    SDL_RenderSetLogicalSize(graphics.render, width, height);
}


HQC_Texture HQC_Artist_LoadTexture(const char* texfile) {
    SDL_Surface* loadedSurface = IMG_Load(texfile);

    if (!loadedSurface) 
        HQC_RaiseErrorHeaderFormat(
            "SDL Image Error",
            "Cannot load image %s [%d]",
            texfile, IMG_GetError()
        );
        
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(graphics.render, loadedSurface);
    if (!newTexture) 
        HQC_RaiseErrorHeaderFormat(
            "SDL Error",
            "Cannot load texture %s [%d]",
            texfile, SDL_GetError()
        );

    SDL_FreeSurface( loadedSurface );
    
    return newTexture;
}


void HQC_Artist_DrawTexture(HQC_Texture texture, float x, float y) {
    if (!texture) _RaiseSDLError("Bad texture");

    SDL_Texture* sdlTexture = (HQC_Texture)texture;

    int w, h;
    SDL_QueryTexture(sdlTexture, NULL, NULL, &w, &h);
    frect_t rect = { 0, 0, w, h };

    HQC_Artist_DrawTextureRect(texture, x, y, rect);
}


void HQC_Artist_DrawTextureRect(HQC_Texture texture, float x, float y, frect_t rect) {
    if (!texture) _RaiseSDLError("Bad texture");

    SDL_Texture* sdlTexture = (HQC_Texture)texture;

    SDL_FRect* sdlFRect = (SDL_FRect*)&rect;
    SDL_FRect  posRect = {
        x - rect.width / 2, y - rect.height / 2,
        rect.width, rect.height
    };

    SDL_RenderCopyF(graphics.render, sdlTexture, sdlFRect, &posRect);
}


void HQC_Artist_Clear() {
    SDL_RenderClear(graphics.render);
}


void HQC_Artist_Display() {
    SDL_RenderPresent(graphics.render);
}


HQC_Event HQC_Window_PollEvent() {
    SDL_Event e;
    return (HQC_Event) SDL_PollEvent(&e);
};