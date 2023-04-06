#ifndef LEVEL_H
#define LEVEL_H

#include <stdbool.h>
#include <string.h>
#include <expat.h>

#include "../global/Engine.h"
#include "../global/Enums.h"

#define PATH_LEVEL "levels"
#define STAGE_COUNT 1
#define BUFF_SIZE   48

#define XML_BUFF_SIZE 32678

typedef struct LevelCoins {
    size_t count;
    v2f_t* posList;
} LevelCoins;

typedef struct LevelGraphics {
    const char* name;

    const char* spiralAFile;
    const char* spiralBFile;

    const char* textureFile;
    const char* textureTopLayerFile;

    const char* dispName;

    v2f_t       frogPos;

    LevelCoins  coins;
} LevelGraphics;

typedef struct LevelSettings {
    const char* name;

    float       ballSpd;
    int         ballStartCount;
    int         gaugeScore;
    int         repeatChance;
    int         singleChance;
    int         ballColors;
    int         partTime;
    float       slowFactor;
} LevelSettings;

typedef struct CurveDot {
    float       dx, dy;
    bool        t1, t2;
} CurveDot;


typedef struct Curve {
    CurveDot*   dotList;
    size_t      dotListCount;
    v2f_t       startPosition;
} Curve;


typedef struct Level {
    LevelSettings** settingsList;
    LevelGraphics*  graphics;

    SDL_Texture* texture;
    SDL_Texture* textureTopLayer;

    Curve* curveA;
    Curve* curveB;
} Level;

int     Level_Load(Level*);
void    Level_Draw(Level*);
void    Level_DrawTopLayer(Level* lvl);
void    Level_DrawDebug(Level*);
char*   Level_GetInfo(Level*);
void    Level_Free(Level*);

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