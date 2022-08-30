#include "Engine.h"

// ==== Common ==== //
void Engine_PushError(const char* header, const char* main) {
    char errMsg[STR_ERR_BUFFER_SIZE];
    sprintf(errMsg, "%s\n%s", header, main);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             "Error",
                             errMsg,
                             NULL);
}

void Engine_PushErrorCode(const char* header, int code) {
    char errMsg[STR_ERR_BUFFER_SIZE];
    sprintf(errMsg, "%s\nError code: %d", header, code);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             "Error",
                             errMsg,
                             NULL);
}

void Engine_PushErrorFile(const char* file, const char* main) {
    char pathMsg[STR_ERR_BUFFER_SIZE];
    sprintf(
        pathMsg, 
        "Error loading file \"%s\".", 
        file
    );
    Engine_PushError(pathMsg, main);
}

void Engine_PushErrorFileCode(const char* file, int code) {
    char pathMsg[STR_ERR_BUFFER_SIZE];
    sprintf(
        pathMsg, 
        "Error loading file \"%s\".", 
        file
    );
    Engine_PushErrorCode(pathMsg, code);
}

int Engine_Init() {
    engine.win = NULL;
    engine.render = NULL;
    engine.textures = NULL;
    engine.texturesLen = 0;

    engine.fullScr = false;

    engine.scale_x = 1.5;
    engine.scale_y = 1.5;

    engine.music = 0;
    engine.sounds = NULL;
    engine.soundsLen = 0;

    engine.volMus = 1.0f;
    engine.volSnd = 1.0f;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        Engine_PushError("SDL.dll graphics library initialization error:", 
                            SDL_GetError());
        return 0;
    }

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (!(IMG_Init(flags) & flags)) {
        Engine_PushError("SDL_Image.dll initialization error:", 
                            IMG_GetError());
        return 0;
    }

   
    if (!BASS_Init(-1, MUSIC_FREQUENCY, BASS_DEVICE_STEREO, 0, NULL)) {
        Engine_PushErrorCode("Bass.dll initialization error", 
                                BASS_ErrorGetCode());
        return 0;
    }

    return 1;
}

void Engine_Destroy() {

    /* Graphics */
    for (int i = 0; i < engine.texturesLen; i++)
        SDL_DestroyTexture(engine.textures[i]);
    free(engine.textures);

    for (int i = 0; i < engine.fontsLen; i++)
        Engine_FontFree(engine.fonts[i]);
    free(engine.fonts);

    SDL_DestroyWindow(engine.win);
    engine.win = NULL;
    SDL_DestroyRenderer(engine.render);
    engine.render = NULL;

    /* Sound and music */
    for (int i = 0; i < engine.soundsLen; i++) 
        BASS_SampleFree(engine.sounds[i]);
    free(engine.sounds);

    for (int i = 0; i < engine.soundsSfxLen; i++) 
        BASS_StreamFree(engine.soundsSfx[i]);
    free(engine.soundsSfx);

    BASS_MusicFree(engine.music);

    SDL_Quit();
    IMG_Quit();
    BASS_Free();
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// ==== Graphics ==== //

int Engine_CreateWindow(const char* cap, int w, int h) {
    engine.win = SDL_CreateWindow(
        cap, 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        w, h, 
        SDL_WINDOW_OPENGL
    );

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    engine.render = SDL_CreateRenderer(
        engine.win, -1, 
        SDL_RENDERER_ACCELERATED | 
        SDL_RENDERER_PRESENTVSYNC 
    );

    if (!engine.render) {
        Engine_PushError("Error while creating render:", 
            SDL_GetError());
        return 0;
    }

    SDL_RenderSetLogicalSize(engine.render, w, h);

    return 1;
}

SDL_Texture* Engine_TextureLoad(const char* fileName) {
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(fileName);
    if(!loadedSurface) {
        Engine_PushErrorFile(fileName, IMG_GetError());
        return NULL;
    } else {
        newTexture = SDL_CreateTextureFromSurface( 
            engine.render, loadedSurface);
        if (!newTexture) 
            Engine_PushErrorFile(fileName, SDL_GetError());

        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}

int Engine_TexturesLoad(const char** files, int n) {
    if (n <= 0)
        return 0;

    engine.textures = malloc(sizeof(SDL_Texture*) * n);
    if (!engine.textures) {
        Engine_PushError("Critical Error!", "Out of memory.");
        return 0;
    }

    char path[STR_PATH_BUFFER_SIZE];
    for (int i = 0; i < n; i++) {
        sprintf(path, "%s\\%s", TEXTURE_FOLDER, files[i]);

        engine.textures[i] = Engine_TextureLoad(path);
        if (!engine.textures[i]) 
            return 0;
    }
    engine.texturesLen = n;

    return 1;
}

SDL_Texture* Engine_GetTextureSDL(int textureID) {
    if (textureID < 0 || textureID >= engine.texturesLen)
        return NULL;

    return engine.textures[textureID];
}


/* Font */
Font* Engine_FontLoad(const char* fileName) {
    Font* font = malloc(sizeof(Font));
    if (!font) {
        Engine_PushErrorFile(fileName, "Out of Memory!");
        return NULL;
    }

    font->texture = NULL;
    font->chars = NULL;
    font->charsLen = 0;
    font->widthList = NULL;
    font->rectList = NULL;
    font->offsetList = NULL;
    font->kerningPairsLen = 0;
    font->kerningPairs = NULL;
    font->kerningValues = NULL;
    
    char path[STR_PATH_BUFFER_SIZE];
    
    sprintf(path, "%s\\%s.png", FONT_FOLDER, fileName);
    font->texture = Engine_TextureLoad(path);
    if (!font->texture) {
        Engine_PushErrorFile(path, "");
        return NULL;
    }

    sprintf(path, "%s\\%s.txt", FONT_FOLDER, fileName);
    FILE* file = fopen(path, "r");
    if (!file) {
        Engine_PushErrorFile(path, "");
        return NULL;
    }

    int n;
    int errFlag = 0;
    char str[64];

    while(fscanf(file, "%s", str) != EOF) {
        if (!fscanf(file, "%d", &n))
            {errFlag = 1; break;}

        if (strcmp(str, "CharList") == 0) {
            font->chars = (char*)malloc(sizeof(char) * n);
            if (!font->chars)
                {errFlag = 1; break;}

            font->charsLen = n;
            for (int i = 0; i < n; i++) {
                char temp;
                if (!fscanf(file, " %c", &temp)) {errFlag = 1; break;}
                font->chars[i] = temp;
            }
        } else if (strcmp(str, "WidthList") == 0) {
            font->widthList = malloc(sizeof(int) * n);
            if (!font->widthList)
                {errFlag = 1; break;}

            for (int i = 0; i < n; i++) {
                int temp;
                if (!fscanf(file, "%d", &temp)) {errFlag = 1; break;}
                font->widthList[i] = temp;
            }
        } else if (strcmp(str, "RectList") == 0) {
            font->rectList = malloc(sizeof(SDL_Rect) * n);
            if (!font->rectList)
                {errFlag = 1; break;}

            for (int i = 0; i < n; i++) {
                SDL_Rect temp;
                if (fscanf(file, "%d %d %d %d", 
                        &temp.x, &temp.y, 
                        &temp.w, &temp.h) != 4) {errFlag = 1; break;}
                font->rectList[i] = temp;
            }
        } else if (strcmp(str, "OffsetList") == 0) {
            font->offsetList = malloc(sizeof(SDL_Point) * n);
            if (!font->offsetList)
                {errFlag = 1; break;}

            for (int i = 0; i < n; i++) {
                SDL_Point temp;
                if (fscanf(file, "%d%d", 
                    &temp.x, &temp.y) != 2) {errFlag = 1; break;}
                font->offsetList[i] = temp;
            }
        } else if (strcmp(str, "KerningPairs") == 0) {
            font->kerningPairs = malloc(sizeof(font->kerningPairs) * n);
            if (!font->kerningPairs)
                {errFlag = 1; break;}

            font->kerningPairsLen = n;

            for (int i = 0; i < n; i++) {
                char temp[2];
                if (!fscanf(file, "%s", &temp)) {errFlag = 1; break;}        
                font->kerningPairs[i].ch[0] = temp[0];
                font->kerningPairs[i].ch[1] = temp[1];
            }
        } else if (strcmp(str, "KerningValues") == 0) {
            font->kerningValues = malloc(sizeof(int) * n);
            if (!font->kerningValues)
                {errFlag = 1; break;}

            for (int i = 0; i < n; i++) {
                int temp;
                if (!fscanf(file, "%d", &temp)) {errFlag = 1; break;}
                font->kerningValues[i] = temp;
            }
        }
    }
    fclose(file);

    if (errFlag) {
        free(font);
        Engine_PushErrorFile(fileName, "");
        return NULL;
    }

    return font;
}

void Engine_FontFree(Font* font) {
    SDL_DestroyTexture(font->texture);
    free(font->chars);
    free(font->widthList);
    free(font->rectList);
    free(font->offsetList);
    free(font->kerningPairs);
    free(font->kerningValues);

    free(font);
}

int Engine_FontsLoad(const char** files, int n) {
    engine.fonts = malloc(sizeof(Font) * n);
    if (!engine.fonts) {
        Engine_PushError("Critical Error!", "Out of memory.");
        return 0;
    }

    for (int i = 0; i < n; i++) {
        engine.fonts[i] = Engine_FontLoad(files[i]);
        if (!engine.fonts[i]) 
            return 0;
    }
    engine.fontsLen = n;

    return 1;
}

/* Draw */
void Engine_DrawTexture(int texID, float x, float y) {
    if (texID < 0 || texID >= engine.texturesLen)
        return;

    int w, h;
    SDL_QueryTexture(engine.textures[texID], NULL, NULL, &w, &h);
    SDL_FRect rect = {x-(w/2), y-(h/2), w, h};

    SDL_RenderCopyF(
        engine.render, 
        engine.textures[texID], 
        NULL, 
        &rect
    );
}

void Engine_DrawTextureWithRot(int texID, float x, float y, float angle) {
    if (texID < 0 || texID >= engine.texturesLen)
        return;

    int w, h;
    SDL_QueryTexture(engine.textures[texID], NULL, NULL, &w, &h);
    SDL_FRect rect = {x-(w/2), y-(h/2), w, h};

    SDL_RenderCopyExF(
        engine.render, 
        engine.textures[texID],
        NULL,
        &rect,
        angle,
        NULL,
        SDL_FLIP_NONE
    );
}

void Engine_DrawTextureSDL(SDL_Texture* tex, float x, float y) {
    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    SDL_FRect rect = {x-(w/2), y-(h/2), w, h};

    SDL_RenderCopyF(
        engine.render, 
        tex, 
        NULL, 
        &rect
    );
}

void Engine_DrawTextScale(const char* str, int fontID, float scale, float x, float y) {
    if (fontID < 0 || fontID >= engine.fontsLen)
        return;
    
    int i = 0;
    int width = 0;
    int height = 0;
    Font* font = engine.fonts[fontID];
    while(str[i] != '\0') {
        int ch = 0;

        if (str[i] == '\n') {
            width = 0;
            height += font->rectList[0].h;
        }
        if (str[i] == ' ')  {
            width += font->widthList[0];
        }

        for (int j = 0; j < font->charsLen; j++) {
            if (str[i] == font->chars[j]) {
                for (int k = 0; k < font->kerningPairsLen; k++) {
                    if (str[i] == font->kerningPairs[k].ch[0] && 
                        str[i+1] == font->kerningPairs[k].ch[1])
                            width += font->kerningValues[k];
                }

                SDL_FRect rect = {
                    x + (width + font->offsetList[j].x) * engine.scale_x * scale, 
                    y + (height+ font->offsetList[j].y) * engine.scale_y * scale, 
                    font->rectList[j].w * engine.scale_x * scale, 
                    font->rectList[j].h * engine.scale_y * scale
                };

                SDL_RenderCopyF(
                    engine.render, 
                    font->texture, 
                    &font->rectList[j], 
                    &rect
                );

                width += font->widthList[j];

                break;
            }
        }
        i++;
    }
}

void Engine_DrawText(const char* str, int fontID, float x, float y) {
    Engine_DrawTextScale(str, fontID, 1, x, y);
}

int Engine_GetTextWidth(const char* str, int fontID) {
    if (fontID < 0 || fontID >= engine.fontsLen)
        return 0; 

    int i = 0;
    int maxWidth = 0;
    int width = 0;
    int dWidth = 0;
    Font* font = engine.fonts[fontID];

    while(str[i] != '\0') {
        int ch = 0;

        if (str[i] == '\n') {
            if (maxWidth < dWidth)
                maxWidth = dWidth;

            width = 0;
        }
        if (str[i] == ' ') {
            width += font->widthList[0];
        }

        for (int j = 0; j < font->charsLen; j++) {
            if (str[i] == font->chars[j]) {
                for (int k = 0; k < font->kerningPairsLen; k++) {
                    if (str[i] == font->kerningPairs[k].ch[0] && 
                        str[i+1] == font->kerningPairs[k].ch[1])
                            width += font->kerningValues[k];
                }

                width += font->widthList[j];
                dWidth = width * engine.scale_x;
                
                break;
            }
        }
        i++;
    }

    if (maxWidth < dWidth)
        maxWidth = dWidth;

    return maxWidth;
}

void Engine_DrawTextExt(const char* str, int fontID, SDL_Color color, bool drawShadow, bool center, float x, float y) {
    Font* font = engine.fonts[fontID];

    int dx = 0;
    if (center)
        dx = Engine_GetTextWidth(str, fontID) / 2;

    if (drawShadow) {
        SDL_SetTextureColorMod(font->texture,
            0, 0, 0);
        SDL_SetTextureAlphaMod(font->texture, 192);
        Engine_DrawText(str, fontID, x-dx+1, y-1);
        SDL_SetTextureAlphaMod(font->texture, 255);
    }
   
    SDL_SetTextureColorMod(font->texture,
        color.r, color.g, color.b);

    Engine_DrawText(str, fontID, x-dx, y);

    SDL_SetTextureColorMod( font->texture,
        255, 255, 255);

}

void Engine_DrawTextExtScale(const char* str, int fontID, float scale, SDL_Color color, bool drawShadow, bool center, float x, float y) {
    Font* font = engine.fonts[fontID];

    int dx = 0;
    if (center)
        dx = (Engine_GetTextWidth(str, fontID) / 2) * scale;

    if (drawShadow) {
        SDL_SetTextureColorMod(font->texture,
            0, 0, 0);
        SDL_SetTextureAlphaMod(font->texture, 192);
        Engine_DrawTextScale(str, fontID, scale, x-dx+1, y-1);
        SDL_SetTextureAlphaMod(font->texture, 255);
    }
   
    SDL_SetTextureColorMod(font->texture,
        color.r, color.g, color.b);

    Engine_DrawTextScale(str, fontID, scale, x-dx, y);

    SDL_SetTextureColorMod( font->texture,
        255, 255, 255);

}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// ==== Sound And Music ==== //

int Engine_MusicLoad(const char* fileName) {
    char path[STR_PATH_BUFFER_SIZE];
    sprintf(path, "%s\\%s", MUSIC_FOLDER, fileName);

    engine.music = BASS_MusicLoad(
        FALSE, path, 0, 0, BASS_SAMPLE_LOOP, MUSIC_FREQUENCY);

    if (engine.music == 0) {
        Engine_PushErrorFileCode(fileName, BASS_ErrorGetCode());
        return 0;
    }

    return 1;
}

HSAMPLE Engine_SoundLoad(const char* fileName) {
    HSAMPLE sound = BASS_SampleLoad(
        FALSE, fileName, 0, 0, 65535, 0);

    if (!sound) {
        Engine_PushErrorFileCode(fileName, BASS_ErrorGetCode());
        return 0;
    }

    return sound;
}

int Engine_SoundsLoad(const char** files, int n) {
    if (n <= 0)
        return 0;

    engine.sounds = (HSAMPLE*)malloc(sizeof(HSAMPLE*) * n);
    if (!engine.sounds) {
        Engine_PushError("Critical Error!", "Out of memory.");
        return 0;
    }

    for (int i = 0; i < n; i++) {
        char path[STR_PATH_BUFFER_SIZE];
        sprintf(path, "%s\\%s", SOUND_FOLDER, files[i]);

        engine.sounds[i] = Engine_SoundLoad(path);
        if (!engine.sounds[i]) 
            return 0;
    }
    engine.soundsLen = n;

    return 1;
}

HSTREAM Engine_SoundSfxLoad(const char* fileName) {
    HSTREAM sound = BASS_StreamCreateFile(
        FALSE, fileName, 0, 0, 
        BASS_STREAM_DECODE);
    sound = BASS_FX_TempoCreate(sound, BASS_SAMPLE_FX);

    if (!sound) {
        Engine_PushErrorFileCode(fileName, BASS_ErrorGetCode());
        return 0;
    }

    return sound;
}

int Engine_SoundsSfxLoad(const char** files, int n) {
    if (n <= 0)
        return 0;

    engine.soundsSfx = (HSTREAM*)malloc(sizeof(HSTREAM*) * n);
    if (!engine.soundsSfx) {
        Engine_PushError("Critical Error!", "Out of memory.");
        return 0;
    }

    for (int i = 0; i < n; i++) {
        char path[STR_PATH_BUFFER_SIZE];
        sprintf(path, "%s\\%s", SOUND_FOLDER, files[i]);

        engine.soundsSfx[i] = Engine_SoundSfxLoad(path);
        if (!engine.soundsSfx[i]) 
            return 0;
    }
    engine.soundsSfxLen = n;

    return 1;
}

HSAMPLE Engine_GetSoundSample(int soundID) {
    if (soundID < 0 || soundID >= engine.soundsLen)
        return 0;

    return engine.sounds[soundID];
}

void Engine_PlayMusic(int musicID) {
    if (!BASS_ChannelIsActive(engine.music)) {
        BASS_ChannelSetAttribute(engine.music, 
            BASS_ATTRIB_VOL, engine.volMus);
        BASS_ChannelPlay(engine.music, FALSE);
    }

    BASS_ChannelSetPosition(
        engine.music, 
        MAKELONG(musicID, 0), 
        BASS_POS_MUSIC_ORDER
    );
}

void Engine_StopMusic() {
    BASS_ChannelStop(engine.music);
}

void Engine_PlaySound(int soundID) {
    if (soundID < 0 || soundID >= engine.soundsLen)
        return;

    HCHANNEL ch = BASS_SampleGetChannel(
        engine.sounds[soundID], FALSE);

    BASS_ChannelSetAttribute(ch, BASS_ATTRIB_VOL, engine.volSnd);
    BASS_ChannelPlay(ch, FALSE);
}

void Engine_StopSound(int soundID) {
    if (soundID < 0 || soundID >= engine.soundsLen)
        return;

    BASS_SampleStop(engine.sounds[soundID]);
}

void Engine_PlaySoundSfxPitch(int soundID, float pitch) {
    BASS_ChannelSetAttribute(engine.soundsSfx[soundID], 
        BASS_ATTRIB_TEMPO_PITCH, pitch);
    BASS_ChannelSetAttribute(engine.soundsSfx[soundID], BASS_ATTRIB_VOL, engine.volSnd);
    BASS_ChannelPlay(engine.soundsSfx[soundID], TRUE);
}

void Engine_StopSoundSfx(int soundID) {
    BASS_ChannelStop(engine.soundsSfx[soundID]);
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/* ==== Misc ==== */

void Engine_GetMousePos(int* mx, int* my) {
    int wWidth, wHeight;
    int _mx, _my;

    SDL_GetMouseState(&_mx, & _my);
    SDL_GetWindowSize(engine.win, &wWidth, &wHeight);
    _mx /= wWidth / WINDOW_WIDTH;
    _my /= wHeight / WINDOW_HEIGHT;

    *mx = _mx;
    *my = _my;
}

int randInt(const int min, const int max) {
    return rand() % (max - min + 1) + min;
}

float fsign(float x) {
	if (x > 0) return 1.0;
	else if (x < 0) return -1.0;
	else return 0;
}

float lerp(float start, float end, float val) {

    return start + val * (end - start);
}

// ==== Saved data ==== //
int Engine_SaveSettings() {
    FILE* file;
    file = fopen("settings.txt", "w");
    if (!file) {
        Engine_PushError("File save error",
            "Unable to write file \"settings.txt\"");
        return 0;
    }

    int mus = (int)(engine.volMus * 100);
    fprintf(file, "MUSIC %d\n", mus);

    int snd = (int)(engine.volSnd * 100);
    fprintf(file, "SOUNDS %d\n", snd);

    fprintf(file, "FULLSCREEN %d", engine.fullScr);

    fclose(file);
    return 1;
}

int Engine_LoadSettings() {
    FILE* file;
    file = fopen("settings.txt", "r");
    if (!file) {
        Engine_SaveSettings();
        return 1;
    }

    char buff[11];
    fscanf(file, "%s", buff);
    if (strcmp(buff, "MUSIC") != 0) {
        Engine_PushError("Failed to load \"settings.txt\"",
            "Invalid file format!");
        fclose(file);
        return 0;
    }

    int mus = -1;
    if (fscanf(file, "%d", &mus) == 1 && mus >= 0 && mus <= 100) {
        engine.volMus = (float)mus / 100.0f;
    } else {
        Engine_PushError("Failed to load \"settings.txt\"",
            "Invalid parameter value MUSIC.");
        fclose(file);
        return 0;
    }

    fscanf(file, "%s", buff);
    if (strcmp(buff, "SOUNDS") != 0) {
        Engine_PushError("Failed to load \"settings.txt\"",
            "Invalid file format!");
        fclose(file);
        return 0;
    }

    int snd = -1;
    if (fscanf(file, "%d", &snd) == 1 && snd >= 0 && snd <= 100) {
        engine.volSnd = (float)snd / 100.0f;
    } else {
        Engine_PushError("Failed to load \"settings.txt\"",
            "Invalid parameter value SOUNDS.");
        fclose(file);
        return 0;
    }

    fscanf(file, "%s", buff);
    if (strcmp(buff, "FULLSCREEN") != 0) {
        Engine_PushError("Failed to load \"settings.txt\"",
            "Invalid file format!");
        fclose(file);
        return 0;
    }

    bool full = -1;
    if (fscanf(file, "%d", &full) == 1 && full >= 0 && full <= 1) {
        engine.fullScr = full;
        if (engine.fullScr) {
            SDL_SetWindowFullscreen(
                engine.win, SDL_WINDOW_FULLSCREEN);
        }
    } else {
        Engine_PushError("Failed to load \"settings.txt\"",
            "Invalid parameter value FULLSCREEN.");
        fclose(file);
        return 0;
    }

    return 1;
}

