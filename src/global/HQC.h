#ifndef HQC_H
#define HQC_H

#include "hqc/Vector.h"
#include "hqc/Typedefs.h"

#include <stdint.h>

typedef void (*HQC_FnCreate) ();
typedef void (*HQC_FnUpdate) (); 

typedef HQC_UInt32 HQC_HTexture;
typedef HQC_UInt32 HQC_HSound;
typedef HQC_UInt32 HQC_HFont;
typedef HQC_UInt32 HQC_HSpriteFont;

typedef struct HQC_Point { HQC_Int x; HQC_Int y; }      HQC_Point;
typedef struct HQC_FPoint { HQC_Float x; HQC_Float y; } HQC_FPoint;

typedef HQC_UInt32 HQC_HList;

void HQC_WindowSetCaption(const char* caption);
void HQC_WindowSetSize(HQC_UInt width, HQC_UInt height);

void HQC_SetFramerateLimit(HQC_UInt32 fpsLimit);

void HQC_TextureLoad();

void HQC_TextureGet();
void HQC_TexturesGetCount();

void HQC_SoundLoad();
void HQC_SoundGet();
void HQC_SoundsGetCount();

void HQC_MusicLoad();
void HQC_MusicGet();
void HQC_MusicPlay();
void HQC_MusicStop();

void HQC_DrawTexture();
void HQC_DrawTextureExt();

void HQC_DrawSetColor();
void HQC_DrawSetBlending();

HQC_Point HQC_MouseGetPos();

// Utils
HQC_Int     HQC_RandomInt();
HQC_Float   HQC_FSign();
HQC_Int     HQC_Sign();

HQC_Float   HQC_Clamp(HQC_Float min, HQC_Float max, HQC_Float val);

HQC_Float   HQC_Lerp(HQC_Float start, HQC_Float end, HQC_Float value);

void HQC_Start(HQC_FnCreate createFunction, HQC_FnUpdate updateFunction);

#endif