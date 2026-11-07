#ifndef SOUND_ENGINE_H
#define SOUND_ENGINE_H

#include <stdbool.h>

typedef struct SoundEngine SoundEngine;

SoundEngine* SoundEngine_Create(int sampleRate);
void SoundEngine_Destroy(SoundEngine* engine);

bool SoundEngine_LoadSounds(SoundEngine* engine, const char* folder,
                            const char** files, int count, int copiesPerSound);
bool SoundEngine_LoadSfx(SoundEngine* engine, const char* folder,
                         const char** files, int count, int copiesPerSound);

void SoundEngine_SetSoundVolume(SoundEngine* engine, float volume);

void SoundEngine_PlaySound(SoundEngine* engine, int soundID);
void SoundEngine_StopSound(SoundEngine* engine, int soundID);

void SoundEngine_PlaySfx(SoundEngine* engine, int sfxID, float pitch);
void SoundEngine_StopSfx(SoundEngine* engine, int sfxID);

#endif
