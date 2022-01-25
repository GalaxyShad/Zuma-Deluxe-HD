#ifndef LEVEL_H
#define LEVEL_H

#include <stdbool.h>
#include <string.h>
#include <expat.h>

#include "Engine.h"
#include "Enums.h"

#define PATH_LEVEL "levels"
#define STAGE_COUNT 1
#define BUFF_SIZE 48

#define XML_BUFF_SIZE 32678

typedef struct _SpiralDot {
    float dx, dy;
    bool t1, t2;
} SpiralDot;

typedef struct _LevelGraphics {
    char id[BUFF_SIZE];

    char spiralFile[BUFF_SIZE];
    char spiral2File[BUFF_SIZE];

    char textureFile[BUFF_SIZE];
    char textureTopLayerFile[BUFF_SIZE];

    char dispName[BUFF_SIZE];

    SDL_FPoint frogPos;

    int coinsLen;
    SDL_FPoint* coinsPos;
} LevelGraphics;

typedef struct _LevelSettings {
    char id[BUFF_SIZE];
    float ballSpd;
    int ballStartCount;
    int gaugeScore;
    int repeatChance;
    int singleChance;
    int ballColors;
    int partTime;
    float slowFactor;
} LevelSettings;

typedef struct _Level {
    int* settingsID;
    int graphicsID;

    SDL_Texture* texture;
    SDL_Texture* textureTopLayer;

    int spiralLen;
    SpiralDot* spiral;
    SDL_FPoint spiralStart;

    int spiral2Len;
    SpiralDot* spiral2;
    SDL_FPoint spiral2Start;
} Level;

int Level_Load(Level*);
void Level_Draw(Level*);
void Level_DrawTopLayer(Level* lvl);
void Level_DrawDebug(Level*);
char* Level_GetInfo(Level*);
void Level_Free(Level*);

typedef struct _Stage {
    int levelsLen;
    Level* levels;
} Stage;

struct {
    int settingsLen;
    LevelSettings* settings;
    int graphicsLen;
    LevelGraphics* graphics;

    unsigned int bestScore[LEVELS_COUNT][4];
    unsigned int bestTime[LEVELS_COUNT][4];

    Stage stages[STAGE_COUNT];
    int survivalLevelsLen;
    Level* survivalLevels;
} levelMgr;

int LevelMgr_LoadLevels(const char*);
int LevelMgr_SaveProgress();
int LevelMgr_LoadProgress();
Stage* LevelMgr_GetStage(int stageID);
Level* LevelMgr_GetLevelFromStage(int stageID, int lvID);
LevelGraphics* LevelMgr_GetLevelGraphics(Level* lv);
LevelSettings* LevelMgr_GetLevelSettings(Level* lv, int settingID);
int LevelMgr_Free();

int xmlDepth;
void XML_StartElement(void *data, const char *element, const char **attribute);
void XML_EndElement(void *data, const char *element);

#endif