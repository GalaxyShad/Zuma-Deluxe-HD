#include "Sound.h"

#include "global/Engine.h"

#include <stdlib.h>
#include <string.h>

#define SOUND_FLAGS (MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_NO_SPATIALIZATION)

static ma_result LoadCopies(Sound* sound, ma_engine* mixer, ma_sound_group* group,
                            const char* path, int copyCount) {
    ma_sound* original = &sound->copies[0];

    ma_result result = ma_sound_init_from_file(mixer, path, SOUND_FLAGS, group, NULL, original);
    if (result != MA_SUCCESS)
        return result;
    sound->copyCount = 1;

    while (sound->copyCount < copyCount) {
        ma_sound* copy = &sound->copies[sound->copyCount];

        result = ma_sound_init_copy(mixer, original, SOUND_FLAGS, group, copy);
        if (result != MA_SUCCESS)
            return result;
        sound->copyCount++;
    }

    return MA_SUCCESS;
}

static ma_sound* TakeNextCopy(Sound* sound) {
    ma_sound* copy = &sound->copies[sound->nextCopy];
    sound->nextCopy = (sound->nextCopy + 1) % sound->copyCount;
    return copy;
}

/////////////////////////////////////////////////////////////////////////

bool Sound_Init(Sound* sound, ma_engine* mixer, ma_sound_group* group,
                const char* path, int copyCount) {
    memset(sound, 0, sizeof(*sound));

    if (copyCount < 1)
        copyCount = 1;

    sound->copies = (ma_sound*)calloc((size_t)copyCount, sizeof(ma_sound));
    if (!sound->copies) {
        Engine_PushError("Critical Error!", "Out of memory.");
        return false;
    }

    ma_result result = LoadCopies(sound, mixer, group, path, copyCount);
    if (result != MA_SUCCESS) {
        Engine_PushErrorFile(path, ma_result_description(result));
        Sound_Uninit(sound);
        return false;
    }

    return true;
}

void Sound_Uninit(Sound* sound) {
    for (int i = 0; i < sound->copyCount; i++)
        ma_sound_uninit(&sound->copies[i]);

    free(sound->copies);
    memset(sound, 0, sizeof(*sound));
}

void Sound_Play(Sound* sound, float pitch) {
    ma_sound* copy = TakeNextCopy(sound);

    ma_sound_stop(copy);
    ma_sound_seek_to_pcm_frame(copy, 0);
    ma_sound_set_pitch(copy, pitch);
    ma_sound_start(copy);
}

void Sound_Stop(Sound* sound) {
    for (int i = 0; i < sound->copyCount; i++)
        ma_sound_stop(&sound->copies[i]);
}
