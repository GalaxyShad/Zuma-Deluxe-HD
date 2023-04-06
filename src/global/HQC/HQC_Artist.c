#include "../HQC.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

static struct {
    SDL_Window*     window;
    SDL_Renderer* 	render;
} graphics;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

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
        HQC_RaiseErrorHeaderFormat(
            "SDL Error", "Rendere creation fail [%d]", SDL_GetError());

    SDL_RenderSetLogicalSize(graphics.render, width, height);
}


bool HQC_Window_PollEvent(HQC_Event* event) {
    SDL_Event sdlEvent;
    bool res = SDL_PollEvent(&sdlEvent);

    *event = (int)sdlEvent.type;

    return res;
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

HQC_Texture HQC_Artist_LoadTexture(const char* texfile) {
    SDL_Surface* loadedSurface = IMG_Load(texfile);

    if (!loadedSurface) {
        HQC_RaiseErrorHeaderFormat(
            "SDL Image Error",
            "Cannot load image %s [%s]",
            texfile, IMG_GetError()
        );
    }
        
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(graphics.render, loadedSurface);
    if (!newTexture) 
        HQC_RaiseErrorHeaderFormat(
            "SDL Error",
            "Cannot load texture %s [%s]",
            texfile, SDL_GetError()
        );

    SDL_FreeSurface( loadedSurface );
    
    return newTexture;
}


void HQC_Artist_DrawTexture(HQC_Texture texture, float x, float y) {
    if (!texture) 
        HQC_RaiseErrorHeaderFormat(
            "HQC_Artist_DrawTexture", 
            "Bad texture [%s]", 
            SDL_GetError());

    SDL_Texture* sdlTexture = (SDL_Texture*)texture;

    int w, h;
    SDL_QueryTexture(sdlTexture, NULL, NULL, &w, &h);
    SDL_FRect rect = { x - w/2, y - h/2, w, h };

    // HQC_Artist_DrawTextureRect(texture, x, y, rect);

    SDL_RenderCopyF(graphics.render, sdlTexture, NULL, &rect);
}


void HQC_Artist_DrawTextureRect(HQC_Texture texture, float x, float y, irect_t rect) {
    if (!texture) 
        HQC_RaiseErrorHeaderFormat(
            "HQC_Artist_DrawTextureRect", 
            "Bad texture [%s]", 
            SDL_GetError());

    SDL_Texture* sdlTexture = (SDL_Texture*)texture;

    SDL_Rect* sdlRect = (SDL_Rect*)&rect;
    SDL_FRect  posRect = {
        x - rect.width / 2, y - rect.height / 2,
        rect.width, rect.height
    };

    SDL_RenderCopyF(graphics.render, sdlTexture, sdlRect, &posRect);
}


void HQC_Artist_DrawLine(float x1, float y1, float x2, float y2) {
    SDL_RenderDrawLineF(graphics.render, x1, y1, x2, y2);
}


void HQC_Artist_DrawPoint(float x, float y) {
    SDL_RenderDrawPointF(graphics.render, x, y);
}


void HQC_Artist_SetColorHex(uint32_t color) {
    SDL_SetRenderDrawColor(
        graphics.render, 
        (color & C_RED)         >> 16,
        (color & C_GREEN)       >> 8,
        (color & C_BLUE),
        (~color & 0xFF000000)   >> 24
    );
}


void HQC_Artist_Clear() {
    SDL_RenderClear(graphics.render);
}


void HQC_Artist_Display() {
    SDL_RenderPresent(graphics.render);
}


