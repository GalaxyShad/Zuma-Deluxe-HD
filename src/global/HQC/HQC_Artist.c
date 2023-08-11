#include "../HQC.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

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


v2i_t   HQC_Input_MouseGetPosition() {
    // int wWidth, wHeight;
    // int _mx, _my;

    v2i_t pos;

    SDL_GetMouseState(&pos.x, &pos.y);
    // SDL_GetWindowSize(graphics.window, &wWidth, &wHeight);
    // _mx /= wWidth / WINDOW_WIDTH;
    // _my /= wHeight / WINDOW_HEIGHT;

    return pos;
}

bool HQC_Input_MouseLeftPressed() {
    return SDL_GetMouseState(NULL, NULL) & 1;
}


bool HQC_Window_PollEvent(HQC_Event* event) {
    SDL_Event sdlEvent;
    bool res = SDL_PollEvent(&sdlEvent);

    *event = (int)sdlEvent.type;

    return res;
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

static float _drawAngle = 0;

void HQC_Artist_DrawSetAngle(float angle) {
    _drawAngle = angle;
}


void HQC_Artist_DrawSetAngleDegrees(float angleInDegrees) {
    HQC_Artist_DrawSetAngle(HQC_DegreesToRadian(angleInDegrees));
}


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


void HQC_Artist_DrawTextureRectLeft(HQC_Texture texture, float x, float y, irect_t rect) {
    if (!texture) 
        HQC_RaiseErrorHeaderFormat(
            "HQC_Artist_DrawTextureRect", 
            "Bad texture [%s]", 
            SDL_GetError());

    SDL_Texture* sdlTexture = (SDL_Texture*)texture;

    SDL_Rect* sdlRect = (SDL_Rect*)&rect;
    SDL_FRect  posRect = {x, y, rect.width, rect.height};

    SDL_RenderCopyExF(
        graphics.render, 
        sdlTexture, 
        sdlRect, 
        &posRect, 
        HQC_RadianToDegrees(_drawAngle),
        NULL,
        SDL_FLIP_NONE
    );
}

void HQC_Artist_DrawTextureRect(HQC_Texture texture, float x, float y, irect_t rect) {
    HQC_Artist_DrawTextureRectLeft(
        texture, 
        x - rect.width / 2, 
        y - rect.height / 2, 
        rect
    );
}


void HQC_Artist_DrawLine(float x1, float y1, float x2, float y2) {
    SDL_RenderDrawLineF(graphics.render, x1, y1, x2, y2);
}


void HQC_Artist_DrawPoint(float x, float y) {
    SDL_RenderDrawPointF(graphics.render, x, y);
}


void HQC_Artist_SetColor(HQC_Color color) {
    SDL_SetRenderDrawColor(
        graphics.render, color.R, color.G, color.B, color.A
    );
}


HQC_Color HQC_Artist_GetColor() {
    HQC_Color color;
    SDL_GetRenderDrawColor(graphics.render, &color.R, &color.G, &color.B, &color.A);
    return color;
}


void HQC_Artist_SetColorHex(uint32_t color) {
    HQC_Color hqcColor;

    hqcColor.R = (color  & C_RED     )  >> 16;
    hqcColor.G = (color  & C_GREEN   )  >> 8;
    hqcColor.B = (color  & C_BLUE    );
    hqcColor.A = (~color & 0xFF000000)  >> 24;

    HQC_Artist_SetColor(hqcColor);
}


uint32_t HQC_Artist_GetColorHex() {
    HQC_Color hqcColor = HQC_Artist_GetColor();

    uint32_t res = 
        ((uint32_t)hqcColor.A << 24) | 
        (hqcColor.R << 16) | 
        (hqcColor.G << 8) | 
        hqcColor.B; 

    return res;
}


void HQC_Artist_Clear() {
    SDL_RenderClear(graphics.render);
}


void HQC_Artist_Display() {
    SDL_RenderPresent(graphics.render);
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

typedef struct Font {
    TTF_Font* ttf;
} Font;


HQC_Font HQC_Font_LoadTrueType(const char* filepath, int size) {
    Font* font = HQC_Memory_Allocate(sizeof(*font));

    font->ttf = TTF_OpenFont(filepath, size);
    if (!font->ttf)
        HQC_RaiseErrorHeaderFormat(
            "HQC_Font_LoadTrueType",
            "Cannot load font %s with size %d",
            filepath, size
        );

    

    return font;
}


void HQC_Artist_DrawText(HQC_Font hfont, const char* text, float x, float y) {
    Font* font = (Font*)hfont;
    
    if (font->ttf == NULL)
        return;
    
    HQC_Color color = HQC_Artist_GetColor();
    SDL_Color sdlColor = { color.R, color.G, color.B, color.A };

    SDL_Surface* surface = TTF_RenderText_Solid(font->ttf, text, sdlColor);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(graphics.render, surface);

    HQC_Artist_DrawTexture(texture, x, y);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


void HQC_Artist_DrawTextShadow(HQC_Font hfont, const char* text, float x, float y) {
    uint32_t pcolor = HQC_Artist_GetColorHex();

    HQC_Artist_SetColorHex(C_BLACK);
    HQC_Artist_DrawText(hfont, text, x+2, y+2);
    HQC_Artist_SetColorHex(pcolor);

    HQC_Artist_DrawText(hfont, text, x, y);
}