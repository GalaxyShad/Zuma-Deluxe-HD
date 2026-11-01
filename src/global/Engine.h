#ifndef ENGINE_H
#define ENGINE_H

#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <bass.h>
#include <stdio.h>
#include <stdbool.h>

#define STR_ERR_BUFFER_SIZE 128
#define STR_PATH_BUFFER_SIZE 48

#define TEXTURE_FOLDER "images"
#define MUSIC_FOLDER "music"
#define SOUND_FOLDER "sounds"
#define FONT_FOLDER "fonts"

#define MUSIC_FREQUENCY 44100

#define WINDOW_WIDTH 1280.0
#define WINDOW_HEIGHT 720.0

typedef struct _Font {
	SDL_Texture* texture;
	int charsLen;
	char* chars;
	int* widthList;
	SDL_Rect* rectList;
	SDL_Point* offsetList;

	int kerningPairsLen;
	struct { char ch[2]; } *kerningPairs;
	int* kerningValues;
} Font;

typedef struct {
    /* Graphics */
    SDL_Window* 	win;
	SDL_Renderer* 	render;
	SDL_Texture** 	textures;
	unsigned int 	texturesLen;

	float 			scale_x;
	float 			scale_y;

	Font** 			fonts;
	unsigned int 	fontsLen;

	bool fullScr;

	/* Scores */


    /* Sound and Music */
	float 			volMus;
	float 			volSnd;

    HMUSIC 			music;
	unsigned int 	soundsLen;
	HSAMPLE* 		sounds;
	unsigned int 	soundsSfxLen;
	HSTREAM*		soundsSfx;
} Engine;

void Engine_PushError(const char*, const char*);
void Engine_PushErrorCode(const char*, int);
void Engine_PushErrorFile(const char*, const char*);
void Engine_PushErrorFileCode(const char*, int);

int Engine_Init(Engine*);
void Engine_Destroy(Engine*);

/* === Graphics === */
int Engine_CreateWindow(Engine*, const char*, int, int);
SDL_Texture* Engine_TextureLoad(Engine*, const char*);
int Engine_TexturesLoad(Engine*, const char**, int);
SDL_Texture* Engine_GetTextureSDL(Engine*, int);

/* Font */
Font* Engine_FontLoad(Engine*, const char*);
int Engine_FontsLoad(Engine*, const char**, int);
void Engine_FontFree(Font*);

/* Drawing */
void Engine_DrawTexture(Engine*, int, float, float);
void Engine_DrawTextureWithRot(Engine*, int, float, float, float);
void Engine_DrawTextureSDL(Engine*, SDL_Texture*, float, float);
void Engine_DrawTextScale(Engine*, const char*, int, float, float, float);
void Engine_DrawText(Engine*, const char*, int, float, float);
int Engine_GetTextWidth(Engine*, const char* str, int fontID);
void Engine_DrawTextExt(Engine*, const char* str, int fontID,
	SDL_Color color, bool drawShadow, bool center,
	float x, float y);
void Engine_DrawTextExtScale(Engine*, const char* str, int fontID,
	float scale, SDL_Color color, bool drawShadow, bool center,
	float x, float y);


/* === Sound and Music === */
int Engine_MusicLoad(Engine*, const char*);

HSAMPLE Engine_SoundLoad(Engine*, const char*);
HSTREAM Engine_SoundSfxLoad(Engine*, const char*);
int Engine_SoundsLoad(Engine*, const char**, int);
int Engine_SoundsSfxLoad(Engine*, const char**, int);
HSAMPLE Engine_GetSoundSample(Engine*, int);

void Engine_PlayMusic(Engine *engine, int);
void Engine_StopMusic();
void Engine_PlaySound(Engine*, int);
void Engine_StopSound(Engine *engine, int);
void Engine_PlaySoundSfxPitch(int, float);
void Engine_StopSoundSfx(int);


void Engine_GetMousePos(Engine *engine, int* mx, int* my);

int Engine_SaveSettings(Engine *engine);
int Engine_LoadSettings(Engine *engine);

int randInt(const int, const int);
float fsign(float x);
float lerp(float start, float end, float val);

#endif
