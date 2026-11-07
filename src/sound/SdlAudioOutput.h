#ifndef SOUND_SDL_AUDIO_OUTPUT_H
#define SOUND_SDL_AUDIO_OUTPUT_H

#include "miniaudio.h"

#include <SDL.h>
#include <stdbool.h>

typedef struct {
    SDL_AudioDeviceID device;
    ma_engine*        mixer;
    int               sampleRate;
    int               channels;
} SdlAudioOutput;

bool SdlAudioOutput_Open(SdlAudioOutput* output, int sampleRate, int channels);

void SdlAudioOutput_Start(SdlAudioOutput* output, ma_engine* mixer);

void SdlAudioOutput_Close(SdlAudioOutput* output);

#endif
