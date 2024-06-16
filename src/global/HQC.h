#pragma once

#ifndef HQC_H
#define HQC_H

#include <stdint.h>
#include <stddef.h>

#include "UsefulTypes.h"

typedef int HQC_Key;

typedef int bool;
#define true  1
#define false 0


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void HQC_Init();
void HQC_Cleanup();

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// Error handling
void HQC_Log(const char* format, ...);
void HQC_RaiseErrorFormat(const char* format, ...);
void HQC_RaiseErrorHeaderFormat(const char* header, const char* format, ...);

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// Window
void HQC_CreateWindow(const char* caption, int width, int height);

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// System
v2i_t   HQC_Input_MouseGetPosition();
bool    HQC_Input_MouseLeftPressed();
bool    HQC_Input_MouseLeft();
HQC_Key HQC_Input_KeyboardGetKey();

void        HQC_Delay(uint32_t ms);
uint32_t    HQC_GetTicks();

float HQC_RadianToDegrees(float radian);
float HQC_DegreesToRadian(float degree);
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// Allocator
void* HQC_Memory_Copy(void* dst, void* src, size_t size);
void* HQC_Memory_Allocate(size_t memSize);
void* HQC_Memory_Reallocate(void* src, size_t newMemSize);
void  HQC_Memory_Free(void* pmem);

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

//Strings
size_t       HQC_StringLength(const char* src);
size_t       HQC_StringSize(const char* src);
const char*  HQC_StringClone(const char* src);
const char*  HQC_StringConcat(const char* strA, const char* strB);

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// Artist
typedef void* HQC_Texture;
typedef void* HQC_Font;

typedef struct HQC_Color {
    uint8_t R, G, B, A;
} HQC_Color;

HQC_Texture HQC_Artist_LoadTexture(const char* texfile);
HQC_Font    HQC_Artist_MakeFontFromTexture();

void HQC_Artist_FreeFont(HQC_Font font);
void HQC_Artist_SetImageFont(HQC_Font font);

void HQC_Artist_DrawSetAlpha(float alpha);
void HQC_Artist_DrawSetScale(float scale);
void HQC_Artist_DrawSetAngle(float angle);
void HQC_Artist_DrawSetAngleDegrees(float angleInDegrees);

void HQC_Artist_DrawTexture(HQC_Texture texture, float x, float y);
void HQC_Artist_DrawTextureRectLeft(HQC_Texture texture, float x, float y, irect_t rect);
void HQC_Artist_DrawTextureRect(HQC_Texture texture, float x, float y, irect_t rect);
void HQC_Artist_DrawLine(float x1, float y1, float x2, float y2);
void HQC_Artist_DrawPoint(float x, float y);


void HQC_Artist_SetColor(HQC_Color color);
HQC_Color  HQC_Artist_GetColor();

void HQC_Artist_SetColorHex(uint32_t color);
uint32_t HQC_Artist_GetColorHex();

#define C_WHITE     0xFFFFFF
#define C_BLACK     0x000000

#define C_RED       0xFF0000
#define C_GREEN     0x00FF00
#define C_BLUE      0x0000FF

#define C_CYAN      0x00FFFF
#define C_MAGNETTA  0xFF00FF
#define C_YELLOW    0xFFFF00

void HQC_Artist_Clear();
void HQC_Artist_Display();


HQC_Font HQC_Font_LoadTrueType(const char* filepath, int size);

void HQC_Artist_DrawText(HQC_Font hfont, const char* text, float x, float y);
void HQC_Artist_DrawTextShadow(HQC_Font hfont, const char* text, float x, float y);

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

enum HQC_Event {
    HQC_EVENT_UNKNOWN,
    HQC_EVENT_QUIT = 256
};

typedef int HQC_Event;

bool HQC_Window_PollEvent(HQC_Event* event);

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// DJ
typedef void* HQC_Sound;
typedef void* HQC_Music;

HQC_Music HQC_DJ_LoadMusic();
HQC_Sound HQC_DJ_LoadSound(const char* filepath);

void HQC_DJ_SetSoundPith(float pitch);

void HQC_DJ_PlayMusic(HQC_Music music);
void HQC_DJ_StopMusic(HQC_Music music);

void HQC_DJ_PlaySound(HQC_Sound sound);
void HQC_DJ_StopSound(HQC_Sound sound);

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// Utils
int   HQC_RandomRange(int min, int max);


int   HQC_Sign(int x);
float HQC_FSign(float x);
float HQC_FAtan2(float y, float x);
float HQC_FSqrt(float x);
float HQC_FSin(float rad);
float HQC_FCos(float rad);
float HQC_PointDistance(float x1, float y1, float x2, float y2);

float HQC_FMin(float a, float b);
float HQC_FMax(float a, float b);

float HQC_Lerp(float start, float end, float val);

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// XML
typedef void* HQC_XmlParser;

typedef void* HQC_XmlParser_StartHandler;
typedef void* HQC_XmlParser_EndHandler;

HQC_XmlParser HQC_XmlParser_Create(const char* encoding);
void HQC_XmlParser_SetHandlers(
    HQC_XmlParser               parser, 
    HQC_XmlParser_StartHandler  startFunction, 
    HQC_XmlParser_EndHandler    endFunction
);
void HQC_XmlParser_Parse(HQC_XmlParser parser, const char* buffer, size_t count, bool isDone);
void HQC_XmlParser_Free(HQC_XmlParser parser);


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

typedef void* HQC_VectorContainer;
#define HQC_VECTOR(T) HQC_VectorContainer

HQC_VectorContainer HQC_Container_CreateVector(size_t elementSize);
void HQC_Container_VectorAdd(HQC_VectorContainer vector, void* element);
void* HQC_Container_VectorGet(HQC_VectorContainer vector, int index);
size_t HQC_Container_VectorCount(HQC_VectorContainer vector);
void HQC_Container_FreeVector(HQC_VectorContainer vector);

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

typedef void* HQC_File;
HQC_File HQC_File_Open(const char* path, const char* mode);

uint8_t HQC_File_ReadByte(HQC_File file);
int16_t HQC_File_ReadInt16(HQC_File file);
int32_t HQC_File_ReadInt32(HQC_File file);
int64_t HQC_File_ReadInt64(HQC_File file);
float   HQC_File_ReadFloat(HQC_File file);

void HQC_File_ReadToBuffer(HQC_File file, void* buffer, size_t size, size_t count);
void HQC_File_Seek(HQC_File file, long offset);
void HQC_File_Close(HQC_File file);

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

typedef void* HQC_Sprite;

void        HQC_Artist_DrawSprite(HQC_Sprite sprite, float x, float y);
HQC_Sprite  HQC_Sprite_Create(HQC_Texture tex, int rx, int ry, int rwidth, int rheight);
irect_t     HQC_Sprite_GetRect(HQC_Sprite hsprite);
HQC_Texture HQC_Sprite_GetTexture(HQC_Sprite hsprite);
void        HQC_Artist_DrawSprite(HQC_Sprite hsprite, float x, float y);
void        HQC_Sprite_Free(HQC_Sprite hsprite);

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

typedef void* HQC_Animation;

HQC_Animation HQC_Animation_Create();
HQC_Animation HQC_Animation_Clone(HQC_Animation hsrc);

void    HQC_Animation_AddFrame(HQC_Animation anim, HQC_Sprite frame);
void    HQC_Animation_SetFrame(HQC_Animation anim, int frame);
size_t  HQC_Animation_FramesCount(HQC_Animation anim);

void    HQC_Animation_Tick(HQC_Animation anim);
void    HQC_Artist_DrawAnimation(HQC_Animation anim, float x, float y);

void    HQC_Animation_SetSpeed(HQC_Animation anim, float spd);
float   HQC_Animation_GetSpeed(HQC_Animation anim);

void HQC_Animation_SetLooping(HQC_Animation anim, bool looping);
bool HQC_Animation_IsEnded(HQC_Animation anim);

void    HQC_Animation_Free(HQC_Animation anim);


#endif // HQC_H

