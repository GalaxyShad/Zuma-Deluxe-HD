#include "Level.h"

static void LevelParser_ParseGraphics(
    void *data, const char *element, const char **attribute
) {

    LevelGraphics* ptr = realloc(
        levelMgr.graphics,
        sizeof(LevelGraphics) * (levelMgr.graphicsLen+1)
    );

    if (!ptr) {
        Engine_PushErrorFile("levels.xml", "Out of Memory!");
        return;
    }

    levelMgr.graphics = ptr;
    levelMgr.graphicsLen++;

    LevelGraphics* g = &levelMgr.graphics[levelMgr.graphicsLen-1];
    strcpy(g->id, "none");
    strcpy(g->spiralFile, "none");
    strcpy(g->spiral2File, "none");
    strcpy(g->textureFile, "none");
    strcpy(g->textureTopLayerFile, "none");
    strcpy(g->dispName, "none");
    g->frogPos.x = 0;
    g->frogPos.y = 0;
    g->coinsLen = 0;
    g->coinsPos = NULL;

    for (int i = 0; attribute[i]; i += 2) {
        if (strcmp(attribute[i], "id") == 0)
            strcpy(g->id, attribute[i+1]);
        else if (strcmp(attribute[i], "curve") == 0)
            strcpy(g->spiralFile, attribute[i+1]);
        else if (strcmp(attribute[i], "curve2") == 0)
            strcpy(g->spiral2File, attribute[i+1]);
        else if (strcmp(attribute[i], "image") == 0) 
            strcpy(g->textureFile, attribute[i+1]);
        else if (strcmp(attribute[i], "image-top") == 0)
            strcpy(g->textureTopLayerFile, attribute[i+1]);
        else if (strcmp(attribute[i], "dispname") == 0)
            strcpy(g->dispName, attribute[i+1]);
        else if (strcmp(attribute[i], "gx") == 0)
            g->frogPos.x = atof(attribute[i+1]);
        else if (strcmp(attribute[i], "gy") == 0)
            g->frogPos.y = atof(attribute[i+1]);
    }

}
    

static void LevelParser_ParseTreasurePoints(
    void *data, const char *element, const char **attribute
) {
    LevelGraphics* g = &levelMgr.graphics[levelMgr.graphicsLen-1];
        
    SDL_FPoint* ptr = (SDL_FPoint*)realloc(
        g->coinsPos,
        sizeof(SDL_FPoint) * (g->coinsLen+1));
    if (!ptr) {
        Engine_PushErrorFile("levels.xml", "Out of Memory!");
        return;
    }

    g->coinsPos = ptr;
    g->coinsLen++;

    for (int i = 0; attribute[i]; i += 2) {
        if (strcmp(attribute[i], "x") == 0)
            g->coinsPos[g->coinsLen-1].x = atof(attribute[i+1]);
        else if (strcmp(attribute[i], "y") == 0)
            g->coinsPos[g->coinsLen-1].y = atof(attribute[i+1]);
    } 
}


static void LevelParser_ParseSettings(
    void *data, const char *element, const char **attribute
) {
    LevelSettings* ptr = realloc(
        levelMgr.settings,
        sizeof(LevelSettings) * (levelMgr.settingsLen+1)
    );

    if (!ptr) {
        Engine_PushErrorFile("levels.xml", "Out of Memory!");
        return;
    }

    levelMgr.settings = ptr;
    levelMgr.settingsLen++;

    LevelSettings* s = &levelMgr.settings[levelMgr.settingsLen-1];

    strcpy(s->id, "none");
    s->ballSpd = 0.5;
    s->ballStartCount = 35;
    s->gaugeScore = 1000;
    s->repeatChance = 40;
    s->singleChance = 6;
    s->ballColors = 4;
    s->partTime = 30;
    s->slowFactor = 4;

    for (int i = 0; attribute[i]; i += 2) {
        if (strcmp(attribute[i], "id") == 0)
            strcpy(s->id, attribute[i+1]);
        else if (strcmp(attribute[i], "speed") == 0)
            s->ballSpd = atof(attribute[i+1]);
        else if (strcmp(attribute[i], "start") == 0)
            s->ballStartCount = atoi(attribute[i+1]);
        else if (strcmp(attribute[i], "score") == 0)
            s->gaugeScore = atoi(attribute[i+1]);
        else if (strcmp(attribute[i], "repeat") == 0)
            s->repeatChance = atoi(attribute[i+1]);
        else if (strcmp(attribute[i], "colors") == 0)
            s->ballColors = atoi(attribute[i+1]);
        else if (strcmp(attribute[i], "single") == 0)
            s->singleChance = atoi(attribute[i+1]);
        else if (strcmp(attribute[i], "partime") == 0)
            s->partTime = atoi(attribute[i+1]);
        else if (strcmp(attribute[i], "slowfactor") == 0)
            s->slowFactor = atof(attribute[i+1]);
    }
}


static void LevelParser_ParseStageProgression(
    void *data, const char *element, const char **attribute
) {

    for (int i = 0; i < STAGE_COUNT; i++)
        levelMgr.stages[i].levelsLen = 0;

    for (int i = 0; attribute[i]; i += 2) {
        if (strncmp(attribute[i], "stage", 5) == 0) {
            char* cid = strtok((char*)attribute[i], "stage");
            int stageID = atoi(cid)-1;

            char* gID = strtok((char*)attribute[i+1], ",");
            while (gID) {
                for (int i = 0; i < levelMgr.graphicsLen; i++) {
                    if (strcmp(gID, levelMgr.graphics[i].id) == 0) {
                        Level* ptr = realloc(
                            levelMgr.stages[stageID].levels,
                            sizeof(Level) * (levelMgr.stages[stageID].levelsLen+1));
                        if (!ptr) {
                            Engine_PushErrorFile("levels.xml", "Out of Memory!");
                            return;
                        }

                        levelMgr.stages[stageID].levels = ptr;
                        levelMgr.stages[stageID].levelsLen++;

                        Level* lv = &levelMgr.stages[stageID].levels[levelMgr.stages[stageID].levelsLen-1];
                        lv->settingsID = NULL;
                        lv->graphicsID = i;
                        break;
                    }
                }
                gID = strtok(NULL, ",");
            }
        } else if (strncmp(attribute[i], "diffi", 5) == 0) {
            char* cid = strtok((char*)attribute[i], "diffi");
            int stageID = atoi(cid)-1;

            char* sID = strtok((char*)attribute[i+1], ",");
            int j = 0;
            while (sID) {
                for (int i = 0; i < levelMgr.settingsLen; i++) {
                    if (strcmp(sID, levelMgr.settings[i].id) == 0) {
                        Level* lv = &levelMgr.stages[stageID].levels[j];
                        lv->settingsID  = malloc(sizeof(int));
                        lv->settingsID[0] = i;
                        break;
                    }
                }
                sID = strtok(NULL, ",");
                j++;
            }
        }
    }
}


void XML_StartElement(void *data, const char *element, const char **attribute) {
    if (strcmp(element, "Graphics") == 0) {
        LevelParser_ParseGraphics(data, element, attribute);
    } else if (xmlDepth == 2 && strcmp(element, "TreasurePoint") == 0) {
        LevelParser_ParseTreasurePoints(data, element, attribute);
    } else if (strcmp(element, "Settings") == 0) {
        LevelParser_ParseSettings(data, element, attribute);
    } else if (strcmp(element, "LevelProgression") == 0) {
        // TODO
    } else if (strcmp(element, "Level") == 0) {
        // TODO
    } else if (strcmp(element, "StageProgression") == 0) {
        LevelParser_ParseStageProgression(data, element, attribute);
    }

    xmlDepth++;
}

void XML_EndElement(void *data, const char *element) {
    xmlDepth--;
}

int LevelMgr_LoadLevels(const char* fileName) {
    levelMgr.graphics = NULL;
    levelMgr.graphicsLen = 0;
    levelMgr.settings = NULL;
    levelMgr.settingsLen = 0;
    levelMgr.survivalLevels = NULL;
    levelMgr.survivalLevelsLen = 0;

    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            levelMgr.bestScore[i][j] = 0;
            levelMgr.bestTime[i][j]  = 0;
        }
    }

    int done;

    char path[STR_PATH_BUFFER_SIZE];
    sprintf(path, "%s\\%s", PATH_LEVEL, fileName);

    char* buff = malloc(XML_BUFF_SIZE);
    if (!buff) {
        Engine_PushErrorFile(path, "Out of memory!");
        return 0;
    }

    FILE *fp = fopen(path, "r");
    if (!fp) {
        Engine_PushErrorFile(path, "Не удалось открыть файл!");
        return 0;
    }

    XML_Parser parser = XML_ParserCreate(NULL);
    XML_SetElementHandler(parser, XML_StartElement, XML_EndElement);

    do {
        int len = (int)fread(buff, 1, sizeof(buff), fp);
        done = len < sizeof(buff);
        if (XML_Parse(parser, buff, len, done) == XML_STATUS_ERROR) {
            Engine_PushErrorFile(path, XML_ErrorString(XML_GetErrorCode(parser)));
            return 0;
        }
    } while(!done);

    free(buff);
    fclose(fp);
    XML_ParserFree(parser);

    if (!LevelMgr_LoadProgress())
        return 0;

    return 1;
}

int LevelMgr_SaveProgress() {
    FILE* file = fopen("progress.bin", "wb");
    if (!file) {
        return 0;
    }

    fputs("zumahdprog", file);
    for (int i = 0; i < LEVELS_COUNT; i++) {
        for (int j = 0; j < 4; j++) {
            fwrite(&levelMgr.bestScore[i][j],
                sizeof(int), 1, file);
            fwrite(&levelMgr.bestTime[i][j], 
                sizeof(int), 1, file);
        }
    }

    fclose(file);
    return 1;
}

int LevelMgr_LoadProgress() {
    FILE* file = fopen("progress.bin", "rb");
    if (!file) {
        if (!LevelMgr_SaveProgress()) {
            Engine_PushError("Error reading file \"progress.bin\".",
                "Could not open file for reading.");
            return 0;
        }
        return 1;
    }

    char buff[11];
    fgets(buff, 11, file);
    if (strcmp(buff, "zumahdprog") != 0) {
        Engine_PushError("Error reading file \"progress.bin\".",
            "Wrong file type.");
        fclose(file);
        return 0;
    }

    for (int i = 0; i < LEVELS_COUNT; i++) {
        for (int j = 0; j < 4; j++) {
            if (fread(&levelMgr.bestScore[i][j],
                sizeof(int), 1, file) != 1) {
                    Engine_PushError("File read error \"progress.bin\".",
                        "The file is damaged.");
                    fclose(file);
                    return 0;
                }
            
            if (fread(&levelMgr.bestTime[i][j], 
                sizeof(int), 1, file) != 1) {
                    Engine_PushError("File read error \"progress.bin\".",
                        "The file is damaged.");
                    fclose(file);
                    return 0;
                }
        }
    }

    fclose(file);
    return 1;
}

Stage* LevelMgr_GetStage(int stageID) {
    if (stageID < 0 || stageID >= STAGE_COUNT)
        return NULL;

    return &levelMgr.stages[stageID];
}

Level* LevelMgr_GetLevelFromStage(int stageID, int lvID) {
    if (stageID < 0 || stageID >= STAGE_COUNT)
        return NULL;

    if (lvID < 0 || lvID >= levelMgr.stages[stageID].levelsLen)
        return NULL;

    return &levelMgr.stages[stageID].levels[lvID];
}

LevelGraphics* LevelMgr_GetLevelGraphics(Level* lv) {
    if (!lv)
        return NULL;

    return &levelMgr.graphics[lv->graphicsID];
}

LevelSettings* LevelMgr_GetLevelSettings(Level* lv, int settingID) {
    if (!lv)
        return NULL;

    if (settingID < 0)
        return NULL;

    return &levelMgr.settings[lv->settingsID[settingID]];
}

int LevelMgr_Free() {
    for (int i = 0; i < levelMgr.graphicsLen; i++)
        free(levelMgr.graphics[i].coinsPos);

    free(levelMgr.graphics);
    free(levelMgr.settings);

    for (int i = 0; i < STAGE_COUNT; i++)
        free(levelMgr.stages[i].levels);
}

////////////////////////////////////
////////////////////////////////////

int Level_Load(Level* level) {
    char path[STR_PATH_BUFFER_SIZE];

    sprintf(
        path, 
        "%s\\%s\\%s.jpg", 
        PATH_LEVEL,
        levelMgr.graphics[level->graphicsID].id,
        levelMgr.graphics[level->graphicsID].textureFile
    );
    level->texture = Engine_TextureLoad(path); 
    if (!level->texture)
        return 0;

    level->textureTopLayer = NULL;
    if (!(strcmp(levelMgr.graphics[level->graphicsID].textureTopLayerFile, "none") == 0)) {
        sprintf(
            path, 
            "%s\\%s\\%s.png", 
            PATH_LEVEL,
            levelMgr.graphics[level->graphicsID].id,
            levelMgr.graphics[level->graphicsID].textureTopLayerFile
        );
        level->textureTopLayer = Engine_TextureLoad(path); 
    }

    level->spiral2 = NULL;
    
    sprintf(
        path, 
        "%s\\%s\\%s.dat", 
        PATH_LEVEL,
        levelMgr.graphics[level->graphicsID].id,
        levelMgr.graphics[level->graphicsID].spiralFile
    );
    FILE* file = fopen(path, "rb");
    if (!file)
        return 0;

    fseek(file, 0x10, SEEK_SET);
    long count;
    fread(&count, sizeof(long), 1, file);
    fseek(file, 0x14 + count * 10, SEEK_SET);

    long c;
    float cx, cy;

    fread(&c, sizeof(long), 1, file);
    fread(&cx, sizeof(float), 1, file);
    fread(&cy, sizeof(float), 1, file);

    level->spiralStart.x = cx;
    level->spiralStart.y = cy;

    level->spiralLen = c-1;
    level->spiral = malloc(sizeof(SpiralDot) * level->spiralLen);

    long i;
    for (i = 0; i < level->spiralLen; i++) {
        char t1, t2, x, y;
        fread(&t1, 1, 1, file);
        fread(&t2, 1, 1, file);
        fread(&x, 1, 1, file);
        fread(&y, 1, 1, file);

        level->spiral[i].t1 = t1;
        level->spiral[i].t2 = t2;
        level->spiral[i].dx = (float)x / 100.0;
        level->spiral[i].dy = (float)y / 100.0;
    }

    fclose(file);
    return 1;
}

void Level_Free(Level* level) {
    SDL_DestroyTexture(level->texture);
    SDL_DestroyTexture(level->textureTopLayer);

    free(level->spiral);
    free(level->spiral2);
}

void Level_Draw(Level* lvl) {
    int w, h;
    SDL_RenderGetLogicalSize(engine.render, &w, &h);
    Engine_DrawTextureSDL(lvl->texture, w / 2, h / 2);
}

void Level_DrawTopLayer(Level* lvl) {
    int w, h;
    SDL_RenderGetLogicalSize(engine.render, &w, &h);
    Engine_DrawTextureSDL(lvl->textureTopLayer, w / 2, h / 2);
}

void Level_DrawDebug(Level* lvl) {
    float sx, sy;
    sx = (104 + lvl->spiralStart.x) * engine.scale_x;
    sy = lvl->spiralStart.y * engine.scale_y;
    SDL_SetRenderDrawColor(engine.render, 255, 255, 255, 160);
    for(int i = 0; i < lvl->spiralLen; i++) {
        if (lvl->spiral[i].t1)
            SDL_SetRenderDrawColor(engine.render,
                0, 255, 0, 255);
        else 
            SDL_SetRenderDrawColor(engine.render,
                255, 255, 255, 255);

        SDL_RenderDrawLineF(
            engine.render, 
            sx, 
            sy, 
            sx+lvl->spiral[i].dx * engine.scale_x, 
            sy+lvl->spiral[i].dy * engine.scale_y
        );

        /*if (lvl->spiral[i].t2) {
            SDL_SetRenderDrawColor(engine.render,
                0, 0, 255, 128);

            SDL_Rect rect = {sx, sy, 16, 16};
            SDL_RenderFillRect(engine.render, &rect);

            SDL_SetRenderDrawColor(engine.render,
                255, 255, 255, 255);
        }*/

        sx = sx+lvl->spiral[i].dx*engine.scale_x;
        sy = sy+lvl->spiral[i].dy*engine.scale_y;
    }

    LevelGraphics* g = LevelMgr_GetLevelGraphics(lvl);
    SDL_SetRenderDrawColor(engine.render, 0, 255, 0, 160);
    for(int i = 0; i < g->coinsLen; i++) {
        SDL_FRect rect = {
            (104 + g->coinsPos[i].x - 16)*engine.scale_x, 
            (g->coinsPos[i].y - 16)*engine.scale_y,
            32,
            32
        };
        SDL_RenderDrawRectF(engine.render, &rect);
    }
}

char* Level_GetInfo(Level* lvl) {
    char* info = malloc(512);

    sprintf(info, " *- Graphics -*\n");
    LevelGraphics* g = LevelMgr_GetLevelGraphics(lvl);
    sprintf(info, "%s id: %s\n", info, g->id);
    sprintf(info, "%s dispName: %s\n", info, g->dispName);
    sprintf(info, "%s spiral: %s\n", info, g->spiralFile);
    sprintf(info, "%s spiral2: %s\n", info, g->spiral2File);
    sprintf(info, "%s texture: %s\n", info, g->textureFile);
    sprintf(info, "%s top texture: %s\n", info, g->textureTopLayerFile);
    sprintf(info, "%s frog x: %lg; y: %lg\n\n", info, g->frogPos.x, g->frogPos.y);

    sprintf(info, "%s *- Settings -*\n", info);
    LevelSettings* s = LevelMgr_GetLevelSettings(lvl, 0);
    sprintf(info, "%s id: %s\n", info, s->id);
    sprintf(info, "%s part time: %d\n", info, s->partTime);
    sprintf(info, "%s repeat chance: %d\n", info, s->repeatChance);
    sprintf(info, "%s single chance: %d\n", info, s->singleChance);
    sprintf(info, "%s slow factor: %f\n", info, s->slowFactor);
    sprintf(info, "%s balls colors: %d\n", info, s->ballColors);
    sprintf(info, "%s balls spd: %f\n", info, s->ballSpd);
    sprintf(info, "%s balls start count: %d\n", info, s->ballStartCount);
    sprintf(info, "%s score: %d\n", info, s->gaugeScore);

    return info;
}