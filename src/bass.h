#pragma once

typedef void* HMUSIC;
typedef void* HSAMPLE;
typedef void* HSTREAM;
typedef void* HCHANNEL;

#define BASS_Init(...) (1)
#define BASS_ErrorGetCode(...) (0)

#define BASS_DEVICE_STEREO 0
#define BASS_STREAM_DECODE 0

#define BASS_SampleLoad(...) ((void*)1)
#define BASS_SampleFree(...) (0)
#define BASS_MusicLoad(...) ((void*)1)
#define BASS_MusicFree(...) (0)
#define BASS_Free(...) (0)
#define BASS_StreamCreateFile(...) ((void*)1)

#define BASS_FX_TempoCreate(...) ((void*)1)
#define BASS_ChannelIsActive(...) (0)
#define BASS_ChannelSetAttribute(...) (0)
#define BASS_ChannelPlay(...) (0)
#define BASS_ChannelSetPosition(...) (0)
#define BASS_ChannelStop(...) (0)
#define BASS_SampleGetChannel(...) (0)
#define BASS_SampleStop(...) (0)

#define BASS_StreamFree(...) (0)

