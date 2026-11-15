#ifndef SOUND_MUSIC_H
#define SOUND_MUSIC_H

#include "miniaudio.h"

#include <libopenmpt/libopenmpt.h>
#include <stdbool.h>

typedef struct {
    ma_data_source_base source;
    openmpt_module*     module;
    int                 sampleRate;
    ma_sound            sound;
} Music;

bool Music_Init(Music* music, ma_engine* mixer, ma_sound_group* group,
                const char* path, int sampleRate);
void Music_Uninit(Music* music);

/* Must be called while the audio device is locked. */
void Music_Play(Music* music, int order);

void Music_Stop(Music* music);
void Music_SetVolume(Music* music, float volume);

#endif
