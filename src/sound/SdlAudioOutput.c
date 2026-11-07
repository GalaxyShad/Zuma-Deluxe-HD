/*
 * Based on the idea of miniaudio_sdl_driver.c by carstene1ns (Carsten Teibes):
 * https://github.com/carstene1ns/Zuma-Deluxe-HD-linux
 */

#include "SdlAudioOutput.h"

#include <string.h>

#define DEVICE_BUFFER_FRAMES 512

static void FillDeviceBuffer(void* userdata, Uint8* buffer, int bufferBytes) {
    SdlAudioOutput* output = (SdlAudioOutput*)userdata;

    ma_uint32 bytesPerFrame = ma_get_bytes_per_frame(ma_format_f32, (ma_uint32)output->channels);
    ma_uint64 framesWanted  = (ma_uint64)bufferBytes / bytesPerFrame;
    ma_uint64 framesRead    = 0;

    ma_engine_read_pcm_frames(output->mixer, buffer, framesWanted, &framesRead);

    size_t bytesWritten = (size_t)(framesRead * bytesPerFrame);
    memset(buffer + bytesWritten, 0, (size_t)bufferBytes - bytesWritten);
}

static SDL_AudioSpec MakeWantedSpec(SdlAudioOutput* output, int sampleRate, int channels) {
    SDL_AudioSpec spec;
    SDL_zero(spec);
    spec.freq     = sampleRate;
    spec.format   = AUDIO_F32SYS;
    spec.channels = (Uint8)channels;
    spec.samples  = DEVICE_BUFFER_FRAMES;
    spec.callback = FillDeviceBuffer;
    spec.userdata = output;
    return spec;
}

/////////////////////////////////////////////////////////////////////////

bool SdlAudioOutput_Open(SdlAudioOutput* output, int sampleRate, int channels) {
    memset(output, 0, sizeof(*output));

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        SDL_LogWarn(SDL_LOG_CATEGORY_AUDIO, "SDL audio init failed: %s", SDL_GetError());
        return false;
    }

    SDL_AudioSpec wanted = MakeWantedSpec(output, sampleRate, channels);
    SDL_AudioSpec granted;

    const int allowedChanges = SDL_AUDIO_ALLOW_FREQUENCY_CHANGE
                             | SDL_AUDIO_ALLOW_SAMPLES_CHANGE;

    output->device = SDL_OpenAudioDevice(NULL, 0, &wanted, &granted, allowedChanges);
    if (output->device == 0) {
        SDL_LogWarn(SDL_LOG_CATEGORY_AUDIO, "No audio device: %s", SDL_GetError());
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return false;
    }

    output->sampleRate = granted.freq;
    output->channels   = granted.channels;
    return true;
}

void SdlAudioOutput_Start(SdlAudioOutput* output, ma_engine* mixer) {
    SDL_LockAudioDevice(output->device);
    output->mixer = mixer;
    SDL_UnlockAudioDevice(output->device);

    SDL_PauseAudioDevice(output->device, 0);
}

void SdlAudioOutput_Close(SdlAudioOutput* output) {
    if (output->device == 0)
        return;

    SDL_CloseAudioDevice(output->device);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    output->device = 0;
    output->mixer  = NULL;
}
