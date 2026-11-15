#ifndef SOUND_SOUND_H
#define SOUND_SOUND_H

#include "miniaudio.h"

#include <stdbool.h>

typedef struct {
    ma_sound* copies;
    int       copyCount;
    int       nextCopy;
} Sound;

bool Sound_Init(Sound* sound, ma_engine* mixer, ma_sound_group* group,
                const char* path, int copyCount);
void Sound_Uninit(Sound* sound);

void Sound_Play(Sound* sound, float pitch);
void Sound_Stop(Sound* sound);

#endif
