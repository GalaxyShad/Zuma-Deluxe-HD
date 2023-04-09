#include "../HQC.h"

#include <bass.h>
#include <bass_fx.h>

HQC_Music HQC_DJ_LoadMusic() {

}

void HQC_DJ_PlayMusic(HQC_Music music) {}
void HQC_DJ_StopMusic(HQC_Music music) {}


HQC_Sound HQC_DJ_LoadSound(const char* filepath) {
    HSTREAM sound = BASS_StreamCreateFile(FALSE, filepath, 0, 0, BASS_STREAM_DECODE);
    sound = BASS_FX_TempoCreate(sound, BASS_SAMPLE_FX);

    if (!sound) 
        HQC_RaiseErrorHeaderFormat("HQC_DJ_LoadSound", "Cannot load sound %s", filepath);

    HSTREAM* out = HQC_Memory_Allocate(sizeof(*out)); 
    *out = sound;
    
    return out;
}

static float _soundPitch = 0;

void HQC_DJ_SetSoundPith(float pitch) {
    _soundPitch = pitch;
}


void HQC_DJ_PlaySound(HQC_Sound sound) {
    HSTREAM snd = *((HSTREAM*)sound);

    BASS_ChannelSetAttribute(snd, BASS_ATTRIB_TEMPO_PITCH, _soundPitch);
    // BASS_ChannelSetAttribute(sound, BASS_ATTRIB_VOL, engine.volSnd);
    BASS_ChannelPlay(snd, TRUE);
}


void HQC_DJ_StopSound(HQC_Sound sound) {

}