#ifndef GAME_H
#define GAME_H

#include <Level.h>
#include <Frog.h>
#include <Bullet.h>
#include <BallChain.h>
#include <FX.h>
#include <MenuMgr.h>

#include <stdlib.h>
#include <time.h>

#define TREASURE_COLLIDE_DIST 2900
#define TREASURE_BLINK_TIME 3
#define TREASURE_LIFE_TIME 10
#define TREASURE_BLINK_SPEED 8
#define TREASURE_FADING_SPEED 0.1
#define TREASURE_MAX_SCALE 2.5

#define SLOWFACTOR_POS 0.8
#define PIT_OPEN_POS 0.2


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

	int treasurePos, totalCoins;
	bool treasureActive, treasureBlinking, treasureFading;
	clock_t treasureTime;
	Animation treasureAnim;

	SDL_FPoint finishPos;
	Animation finishAnim;

	SDL_Rect scoreRect;

	Button btnMenu;

	DialogueBox* dbMenu;
	DialogueBox* dbStats;
	DialogueBox* dbGameOver;

} Game;

void Game_Init(Game*, int, int);

void Game_Update(Game*, int*, int);
void Game_UpdateTreasure(Game*);
void Game_UpdateOutro(Game*);
void Game_UpdateIntro(Game*);

void Game_Draw(Game*);
void Game_DrawTreasure(Game*);
void Game_DrawOutro(Game*);
void Game_DrawIntro(Game*);
void Game_DrawHUD(Game*);
void Game_DrawFinish(Game*);

void constrain(int*, int, int);

#endif