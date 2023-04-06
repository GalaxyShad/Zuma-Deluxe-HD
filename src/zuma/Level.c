#include "Level.h"

typedef struct GauntLevelProgression {
    const char*                 id;
    HQC_VECTOR(LevelSettings)   settingsList;
    HQC_VECTOR(int)             difficultyList;
} GauntLevelProgression;


typedef struct GauntLevel {
    LevelGraphics*           graphics;
    GauntLevelProgression*   progression;
} GauntLevel;


typedef struct StageLevel {
    LevelGraphics*      graphics;
    LevelSettings*      difficulty;
} StageLevel;


typedef struct StageProgression {
    HQC_VECTOR(StageLevel) stageList;
} StageProgression;

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

typedef struct CurveDot {
    float       x,  y;
    bool        t1, t2;
} CurveDot;


typedef struct Curve {
    HQC_VECTOR(CurveDot) dotList;
    v2f_t                startPosition;
} Curve;


static Curve* _CurveLoadFromFile(const char* filepath) {
    HQC_File file = HQC_File_Open(filepath, "rb");

    // Skip header section
    HQC_File_Seek(file, 0x10);
    
    // Skip first part of file
    uint32_t count = HQC_File_ReadInt32(file);
    HQC_File_Seek(file, 0x14 + count * 10);

    uint32_t curveLength = HQC_File_ReadInt32(file);

    Curve* curve = HQC_Memory_Allocate(sizeof(*curve));
    curve->startPosition.x = HQC_File_ReadFloat(file);
    curve->startPosition.y = HQC_File_ReadFloat(file);
    curve->dotList         = HQC_Container_CreateVector(sizeof(CurveDot));

    float xprev = curve->startPosition.x;
    float yprev = curve->startPosition.y;

    for (uint32_t i = 0; i < curveLength; i++) {
        CurveDot curveDot;

        curveDot.t1 = HQC_File_ReadByte(file);
        curveDot.t2 = HQC_File_ReadByte(file);

        char dx = (char)HQC_File_ReadByte(file);
        char dy = (char)HQC_File_ReadByte(file);

        xprev += dx / 100.0f;
        yprev += dy / 100.0f;

        curveDot.x = xprev;
        curveDot.y = yprev;

        HQC_Container_VectorAdd(curve->dotList, &curveDot);
    }

    HQC_File_Close(file);

    return curve;
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

typedef struct Level {
    LevelSettings* settings;
    LevelGraphics* graphics;

    HQC_Texture texture;
    HQC_Texture textureTopLevel;

    Curve* curveA;
    Curve* curveB;
} Level;


HLevel Level_Load(LevelSettings* settings, LevelGraphics* graphics) {
    Level* level = HQC_Memory_Allocate(sizeof(*level));

    level->settings = settings;
    level->graphics = graphics;

    level->texture         = HQC_Artist_LoadTexture(graphics->textureFile);
    level->textureTopLevel = (graphics->textureTopLayerFile != NULL) ? 
                                HQC_Artist_LoadTexture(graphics->textureTopLayerFile) :
                                NULL;


    level->curveA = _CurveLoadFromFile(graphics->curveAFile);
    level->curveB = (graphics->curveBFile != NULL) ? 
                        _CurveLoadFromFile(graphics->curveBFile) :
                        NULL;

    return level;
}


void Level_Draw(HLevel hlevel, float x, float y) {
    Level* level = (Level*)hlevel;

    HQC_Artist_DrawTexture(level->texture, x, y);

    size_t count = HQC_Container_VectorCount(level->curveA->dotList);

    HQC_Artist_SetColorHex(C_GREEN);
    for (int i = 1; i < count; i++) {
        CurveDot* pdot = HQC_Container_VectorGet(level->curveA->dotList, i-1);
        CurveDot* dot  = HQC_Container_VectorGet(level->curveA->dotList, i);
        HQC_Artist_DrawLine((pdot->x + 104) * 1.5, pdot->y * 1.5, (dot->x + 104) * 1.5, dot->y * 1.5);
    }
    HQC_Artist_SetColorHex(C_WHITE);
}