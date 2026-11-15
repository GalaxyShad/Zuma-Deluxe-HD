#include "SoundEngine.h"

#include "Music.h"
#include "SdlAudioOutput.h"
#include "Sound.h"
#include "miniaudio.h"
#include "miniaudio_libvorbis.h"

#include "global/Engine.h"

#include <SDL.h>
#include <math.h>
#include <stdlib.h>

#define OUTPUT_CHANNELS 2
#define ORIGINAL_PITCH  1.0f

typedef struct {
    Sound* items;
    int    count;
} SoundList;

struct SoundEngine {
    SdlAudioOutput      output;
    ma_resource_manager resources;
    ma_engine           mixer;
    ma_sound_group      soundGroup;

    SoundList           sounds;
    SoundList           sfx;
    Music               music;
};

/////////////////////////////////////////////////////////////////////////
// ==== Sound list ==== //

static void SoundList_Free(SoundList* list) {
    for (int i = 0; i < list->count; i++)
        Sound_Uninit(&list->items[i]);

    free(list->items);
    list->items = NULL;
    list->count = 0;
}

static bool SoundList_Load(SoundList* list, SoundEngine* engine, const char* folder,
                           const char** files, int count, int copiesPerSound) {
    SoundList_Free(list);

    list->items = (Sound*)calloc((size_t)count, sizeof(Sound));
    if (!list->items) {
        Engine_PushError("Critical Error!", "Out of memory.");
        return false;
    }

    for (int i = 0; i < count; i++) {
        char path[512];
        SDL_snprintf(path, sizeof(path), "%s/%s", folder, files[i]);

        if (!Sound_Init(&list->items[i], &engine->mixer, &engine->soundGroup,
                        path, copiesPerSound)) {
            SoundList_Free(list);
            return false;
        }
        list->count = i + 1;
    }

    return true;
}

static Sound* SoundList_Get(SoundList* list, int id) {
    if (id < 0 || id >= list->count)
        return NULL;

    return &list->items[id];
}

/////////////////////////////////////////////////////////////////////////
// ==== Audio setup ==== //

static float SemitonesToSpeedRatio(float semitones) {
    return powf(2.0f, semitones / 12.0f);
}

static bool InitResources(SoundEngine* engine) {
    ma_decoding_backend_vtable* decoders[] = { ma_decoding_backend_libvorbis };

    ma_resource_manager_config config = ma_resource_manager_config_init();
    config.ppCustomDecodingBackendVTables = decoders;
    config.customDecodingBackendCount     = 1;

    return ma_resource_manager_init(&config, &engine->resources) == MA_SUCCESS;
}

static bool InitMixer(SoundEngine* engine) {
    ma_engine_config config = ma_engine_config_init();
    config.pResourceManager = &engine->resources;
    config.sampleRate       = (ma_uint32)engine->output.sampleRate;
    config.channels         = (ma_uint32)engine->output.channels;

    return ma_engine_init(&config, &engine->mixer) == MA_SUCCESS;
}

static bool InitSoundGroup(SoundEngine* engine) {
    return ma_sound_group_init(&engine->mixer, 0, NULL, &engine->soundGroup) == MA_SUCCESS;
}

static bool StartAudio(SoundEngine* engine, int sampleRate) {
    if (!SdlAudioOutput_Open(&engine->output, sampleRate, OUTPUT_CHANNELS))
        return false;

    if (!InitResources(engine))
        goto closeOutput;
    if (!InitMixer(engine))
        goto freeResources;
    if (!InitSoundGroup(engine))
        goto freeMixer;

    SdlAudioOutput_Start(&engine->output, &engine->mixer);
    return true;

freeMixer:
    ma_engine_uninit(&engine->mixer);
freeResources:
    ma_resource_manager_uninit(&engine->resources);
closeOutput:
    SdlAudioOutput_Close(&engine->output);
    return false;
}

/////////////////////////////////////////////////////////////////////////
// ==== Lifetime ==== //

SoundEngine* SoundEngine_Create(int sampleRate) {
    SoundEngine* engine = (SoundEngine*)calloc(1, sizeof(SoundEngine));
    if (!engine)
        return NULL;

    if (!StartAudio(engine, sampleRate)) {
        SDL_LogWarn(SDL_LOG_CATEGORY_AUDIO, "Running without sound.");
        free(engine);
        return NULL;
    }

    return engine;
}

void SoundEngine_Destroy(SoundEngine* engine) {
    if (!engine)
        return;

    SdlAudioOutput_Close(&engine->output);

    SoundList_Free(&engine->sounds);
    SoundList_Free(&engine->sfx);
    Music_Uninit(&engine->music);

    ma_sound_group_uninit(&engine->soundGroup);
    ma_engine_uninit(&engine->mixer);
    ma_resource_manager_uninit(&engine->resources);

    free(engine);
}

/////////////////////////////////////////////////////////////////////////
// ==== Loading ==== //

bool SoundEngine_LoadSounds(SoundEngine* engine, const char* folder,
                            const char** files, int count, int copiesPerSound) {
    if (!engine)
        return true;

    return SoundList_Load(&engine->sounds, engine, folder, files, count, copiesPerSound);
}

bool SoundEngine_LoadSfx(SoundEngine* engine, const char* folder,
                         const char** files, int count, int copiesPerSound) {
    if (!engine)
        return true;

    return SoundList_Load(&engine->sfx, engine, folder, files, count, copiesPerSound);
}

bool SoundEngine_LoadMusic(SoundEngine* engine, const char* path) {
    if (!engine)
        return true;

    Music_Uninit(&engine->music);
    return Music_Init(&engine->music, &engine->mixer, NULL, path, engine->output.sampleRate);
}

/////////////////////////////////////////////////////////////////////////
// ==== Playback ==== //

void SoundEngine_SetSoundVolume(SoundEngine* engine, float volume) {
    if (engine)
        ma_sound_group_set_volume(&engine->soundGroup, volume);
}

void SoundEngine_PlaySound(SoundEngine* engine, int soundID) {
    if (!engine)
        return;

    Sound* sound = SoundList_Get(&engine->sounds, soundID);
    if (sound)
        Sound_Play(sound, ORIGINAL_PITCH);
}

void SoundEngine_StopSound(SoundEngine* engine, int soundID) {
    if (!engine)
        return;

    Sound* sound = SoundList_Get(&engine->sounds, soundID);
    if (sound)
        Sound_Stop(sound);
}

void SoundEngine_PlaySfx(SoundEngine* engine, int sfxID, float pitch) {
    if (!engine)
        return;

    Sound* sound = SoundList_Get(&engine->sfx, sfxID);
    if (sound)
        Sound_Play(sound, SemitonesToSpeedRatio(pitch));
}

void SoundEngine_StopSfx(SoundEngine* engine, int sfxID) {
    if (!engine)
        return;

    Sound* sound = SoundList_Get(&engine->sfx, sfxID);
    if (sound)
        Sound_Stop(sound);
}

void SoundEngine_PlayMusic(SoundEngine* engine, int order) {
    if (!engine)
        return;

    SDL_LockAudioDevice(engine->output.device);
    Music_Play(&engine->music, order);
    SDL_UnlockAudioDevice(engine->output.device);
}

void SoundEngine_StopMusic(SoundEngine* engine) {
    if (engine)
        Music_Stop(&engine->music);
}

void SoundEngine_SetMusicVolume(SoundEngine* engine, float volume) {
    if (engine)
        Music_SetVolume(&engine->music, volume);
}
