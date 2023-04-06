#ifndef HQC_H
#define HQC_H

#include <stdint.h>

#include "UsefulTypes.h"

typedef void* HQC_Texture;
typedef void* HQC_Sound;
typedef void* HQC_Music;
typedef void* HQC_Font;
typedef void* HQC_Sprite;

typedef int HQC_Key;

typedef int bool;
#define true  1
#define false 0


void HQC_Init();
void HQC_Cleanup();

// Error handling
void HQC_RaiseErrorFormat(const char* format, ...);
void HQC_RaiseErrorHeaderFormat(const char* header, const char* format, ...);

// Window
void HQC_CreateWindow(const char* caption, int width, int height);

// System
v2i_t   HQC_Input_MouseGetPosition();
bool    HQC_Input_MouseButtonIsClicked();
HQC_Key HQC_Input_KeyboardGetKey();

void        HQC_Delay(uint32_t ms) {
    SDL_Delay(ms);
}
uint32_t    HQC_GetTicks() {
    return SDL_GetTicks();
}

// Allocator
void* HQC_Memory_Allocate(size_t memSize);
void* HQC_Memory_Reallocate(void* src, size_t newMemSize);
void  HQC_Memory_Free(void* pmem);

// Strings
size_t HQC_StringLength(const char* src) {
    if (src == NULL)
        HQC_RaiseErrorFormat("[HQC_StringLength] Bad string");

    size_t counter = 0;
    for (int i = 0; i < 32687; i++) {
        counter++;
        if (src[i] == '\0') return counter;
    }

    HQC_RaiseErrorFormat("[HQC_StringLength] Buffer overflow. No null terminator found");
}

const char*  HQC_StringClone(const char* src) {
    size_t size = HQC_StringLength(src);

    return (const char*)HQC_Allocate(size);
}

// Artist
HQC_Texture HQC_Artist_LoadTexture(const char* texfile);

HQC_Font    HQC_Artist_MakeFontFromTexture();

void HQC_Artist_FreeFont(HQC_Font font);
void HQC_Artist_SetImageFont(HQC_Font font);

void HQC_Artist_DrawTexture(HQC_Texture texture, float x, float y);
void HQC_Artist_DrawTextureRect(HQC_Texture texture, float x, float y, frect_t rect);
void HQC_Artist_DrawSprite(HQC_Sprite sprite);
void HQC_Artist_DrawText();
void HQC_Artist_Clear();
void HQC_Artist_Display();


typedef enum HQC_Event {
    HQC_EVENT_QUIT = 256
} HQC_Event;

HQC_Event HQC_Window_PollEvent();

// DJ
HQC_Music HQC_DJ_LoadMusic();
HQC_Sound HQC_DJ_LoadSound();

void HQC_DJ_PlayMusic(HQC_Music music);
void HQC_DJ_StopMusic(HQC_Music music);

void HQC_DJ_PlaySound(HQC_Sound sound);
void HQC_DJ_StopSound(HQC_Sound sound);

// Utils
int   HQC_RandomRange(int min, int max);

int   HQC_Sign(int x);
float HQC_FSign(float x);
float HQC_Lerp(float start, float end, float val);


#endif