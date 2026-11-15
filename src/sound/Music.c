#include "Music.h"

#include "global/Engine.h"

#include <SDL.h>
#include <string.h>

#define MUSIC_CHANNELS 2
#define LOOP_FOREVER   -1

static ma_result ReadFrames(ma_data_source* source, void* out,
                            ma_uint64 frameCount, ma_uint64* framesRead) {
    Music* music = (Music*)source;

    size_t read = openmpt_module_read_interleaved_float_stereo(
        music->module, music->sampleRate, (size_t)frameCount, (float*)out);

    if (framesRead)
        *framesRead = read;

    return read == 0 ? MA_AT_END : MA_SUCCESS;
}

static ma_result GetDataFormat(ma_data_source* source, ma_format* format,
                               ma_uint32* channels, ma_uint32* sampleRate,
                               ma_channel* channelMap, size_t channelMapCap) {
    Music* music = (Music*)source;

    *format     = ma_format_f32;
    *channels   = MUSIC_CHANNELS;
    *sampleRate = (ma_uint32)music->sampleRate;
    ma_channel_map_init_standard(ma_standard_channel_map_default,
                                 channelMap, channelMapCap, MUSIC_CHANNELS);
    return MA_SUCCESS;
}

static ma_data_source_vtable musicSourceVtable = {
    ReadFrames,
    NULL,
    GetDataFormat,
    NULL,
    NULL,
    NULL,
    0
};

static openmpt_module* LoadModule(const char* path) {
    size_t size = 0;
    void* data = SDL_LoadFile(path, &size);
    if (!data) {
        Engine_PushErrorFile(path, SDL_GetError());
        return NULL;
    }

    int error = OPENMPT_ERROR_OK;
    const char* errorText = NULL;

    openmpt_module* module = openmpt_module_create_from_memory2(
        data, size, openmpt_log_func_silent, NULL,
        openmpt_error_func_ignore, NULL, &error, &errorText, NULL);
    SDL_free(data);

    if (!module)
        Engine_PushErrorFile(path, errorText ? errorText : "Unknown music format.");

    openmpt_free_string(errorText);
    return module;
}

/////////////////////////////////////////////////////////////////////////

bool Music_Init(Music* music, ma_engine* mixer, ma_sound_group* group,
                const char* path, int sampleRate) {
    memset(music, 0, sizeof(*music));

    music->module = LoadModule(path);
    if (!music->module)
        return false;

    openmpt_module_set_repeat_count(music->module, LOOP_FOREVER);
    music->sampleRate = sampleRate;

    ma_data_source_config config = ma_data_source_config_init();
    config.vtable = &musicSourceVtable;

    ma_result result = ma_data_source_init(&config, &music->source);
    if (result == MA_SUCCESS)
        result = ma_sound_init_from_data_source(mixer, &music->source,
                                                MA_SOUND_FLAG_NO_SPATIALIZATION,
                                                group, &music->sound);

    if (result != MA_SUCCESS) {
        Engine_PushErrorFile(path, ma_result_description(result));
        ma_data_source_uninit(&music->source);
        openmpt_module_destroy(music->module);
        memset(music, 0, sizeof(*music));
        return false;
    }

    return true;
}

void Music_Uninit(Music* music) {
    if (!music->module)
        return;

    ma_sound_uninit(&music->sound);
    ma_data_source_uninit(&music->source);
    openmpt_module_destroy(music->module);
    memset(music, 0, sizeof(*music));
}

void Music_Play(Music* music, int order) {
    if (!music->module)
        return;

    openmpt_module_set_position_order_row(music->module, order, 0);
    ma_sound_start(&music->sound);
}

void Music_Stop(Music* music) {
    if (music->module)
        ma_sound_stop(&music->sound);
}

void Music_SetVolume(Music* music, float volume) {
    if (music->module)
        ma_sound_set_volume(&music->sound, volume);
}
