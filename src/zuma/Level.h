#pragma once

#include "../global/HQC.h"

typedef struct LevelGraphics {
    const char*         id;

    const char*         curveAFile;
    const char*         curveBFile;

    const char*         textureFile;
    const char*         textureTopLayerFile;

    const char*         dispName;

    v2f_t               frogPos;

    HQC_VECTOR(v2f_t)   coinsPosList;
} LevelGraphics;


typedef struct LevelSettings {
    const char*         id;

    float               ballSpd;
    int                 ballStartCount;
    int                 gaugeScore;
    int                 repeatChance;
    int                 singleChance;
    int                 ballColors;
    int                 partTime;
    
    float               slowFactor;
} LevelSettings;


typedef void* HLevel;

HLevel Level_Load(LevelSettings* settings, LevelGraphics* graphics);
const char* Level_GetDisplayName(HLevel hlevel);
void Level_Draw(HLevel hlevel, float x, float y);