#ifndef GAME_H
#define GAME_H

#include <stdlib.h>
#include <time.h>

#include "../menu/MenuMgr.h"

#include "objects/Frog.h"
#include "objects/Bullet.h"
#include "objects/BallChain.h"
#include "objects/FX.h"

#include "Level.h"

#define TREASURE_COLLIDE_DIST 2900
#define TREASURE_BLINK_TIME 3
#define TREASURE_LIFE_TIME 10
#define TREASURE_BLINK_SPEED 8
#define TREASURE_FADING_SPEED 0.1
#define TREASURE_MAX_SCALE 2.5

#define SLOWFACTOR_POS 0.8
#define PIT_OPEN_POS 0.2


typedef struct _Treasure {
	int pos;
	int totalCoins;

	bool isActive;
	bool isBlinking; 
	bool isFading;

	clock_t   time;
	Animation anim;
} Treasure;
typedef struct _Game {
	int mx, my, stageID, lvlID, score, lives, difficulty;
	int ballsOnStart;
	bool isRunning, DEBUG;
	bool isFirstTime, isLosed, isWon, isIntroEnded, isOutroEnded;
	float headerFontScale, subHeaderFontScale;

	BallChain chain;
	BulletsArr bullets;
	Messages msgs;
	Particles particles;

	Frog frog;
	int startLen, counter;
	int time;
	float lightTrailTime;
	float lightTrailPitch;
	Level* lvl;
	LevelSettings* settings;
	LevelGraphics* graphics;

	int totalCoins;
	Treasure treasure;

	SDL_FPoint finishPos;
	Animation finishAnim;

	SDL_Rect scoreRect;

	Button btnMenu;

	DialogueBox* dbMenu;
	DialogueBox* dbStats;
	DialogueBox* dbGameOver;

} Game;



void Game_Init(LevelMgr*, Engine*, Game*, int, int);

void Game_Update(LevelMgr*, Engine*, Game*, int*, int);
void Game_UpdateTreasure(Engine*, Game*);
void Game_UpdateOutro(Engine*, Game*);
void Game_UpdateIntro(Engine*, Game*);

void Game_Draw(LevelMgr*, Engine*, Game*);
void Game_DrawTreasure(Engine*, Game*);
void Game_DrawOutro(Engine*, Game*);
void Game_DrawIntro(Engine*, Game*);
void Game_DrawHUD(Engine*, Game*);
void Game_DrawFinish(Engine*, Game*);

void constrain(int*, int, int);

#endif